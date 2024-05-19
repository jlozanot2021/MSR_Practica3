#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

class SpeedController : public rclcpp::Node
{
public:
    SpeedController() : Node("speed_controller")
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(
            "/rover_base_control/cmd_vel_unstamped", 10);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(30),
                                          std::bind(&SpeedController::timer_callback, this));
        start_time_ = this->now().seconds();
    }

private:
    void timer_callback()
    {
        double elapsed_time = this->now().seconds() - start_time_;

        geometry_msgs::msg::Twist twist;
        if (elapsed_time <= 5.0)
        {
            twist.linear.x = elapsed_time; // velocidad de 0 a 5 los primeros 5 segundos
        }
        else if (elapsed_time <= 10.0)
        {
            twist.linear.x = 5.0; // velocidad estable en 5 de 5 a 10 segundos
        }
        else
        {
            double speed = 5.0 - (elapsed_time - 10.0);
            twist.linear.x = std::max(0.0, speed); // velocidad descendente de 5 a 0 desde los 10 a los 15 segundos
        }

        publisher_->publish(twist);
        RCLCPP_INFO(get_logger(), "Speed: %.2f", twist.linear.x);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    double start_time_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SpeedController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
