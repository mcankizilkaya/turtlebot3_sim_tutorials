#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

#include <memory>
#include <utility>

using namespace std::chrono_literals;

class AvoidObstacle : public rclcpp::Node 
{

public:
    AvoidObstacle()
    : Node("avoid_obstacle_node")
    {
        // Subscribe to sensor messages
        auto sensor_qos = rclcpp::QoS(rclcpp::SensorDataQoS());
        subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>("scan", \
          sensor_qos, \
            std::bind(
            &AvoidObstacle::scan_callback, \
            this, \
            std::placeholders::_1));

        // Advertise velocity commands
        auto default_qos = rclcpp::QoS(rclcpp::SystemDefaultsQoS());
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", default_qos);

    }
    ~AvoidObstacle()
    {
        RCLCPP_INFO(this->get_logger(), "Avoid Obstacle node has been terminated");
    }

private:
    // ROS2 topic publisher
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;

    //ROS2 topic subscribers
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_;

    double scan_data_[5];

    // Function
    void scan_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        uint16_t scan_angle[5] = {0, 30, 60, 300, 330};

        for(int num = 0; num<5; num++)
        {
            if(std::isinf(msg->ranges.at(scan_angle[num])))
            {
                scan_data_[num] = msg->range_max;
            }
            else
            {
                scan_data_[num] = msg->ranges.at(scan_angle[num]);
            }
        }
        // Define a threshold distance for obstacle detection
        const float threshold_distance = 0.4;  // 40 cm

        geometry_msgs::msg::Twist cmd_vel_msg;

        // Obstacle avoidance 

        if(scan_data_[0] < threshold_distance) {

            cmd_vel_msg.linear.x = 0.0;
            if((scan_data_[3] > threshold_distance) && (scan_data_[4] > threshold_distance)) {
                cmd_vel_msg.angular.z = -0.5; // Turn right if the right side is clear
            } else if((scan_data_[1] > threshold_distance) && (scan_data_[2] > threshold_distance)) {
                cmd_vel_msg.angular.z = 0.5;  // Turn left if the left side is clear
            } else {
                // If no clear side, rotate in place to search for an opening
                cmd_vel_msg.angular.z = 0.5;
            }
        } else if ((scan_data_[1] < threshold_distance) || (scan_data_[2] < threshold_distance)) {
            // Obstacle on the left side
            cmd_vel_msg.linear.x = 0.0;   // Stop forward motion
            cmd_vel_msg.angular.z = -0.5; // Turn right to avoid
        } else if ((scan_data_[3] < threshold_distance) || (scan_data_[4] < threshold_distance)) {
            // Obstacle on the right side
            cmd_vel_msg.linear.x = 0.0;   // Stop forward motion
            cmd_vel_msg.angular.z = 0.5;  // Turn left to avoid
        } else {
            // Path is clear, move forward
            cmd_vel_msg.linear.x = 0.2;   // Move forward
            cmd_vel_msg.angular.z = 0.0; // Keep heading straight
  }

        //Publish cmd_vel
        publisher_->publish(cmd_vel_msg);

    }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<AvoidObstacle>();

    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}