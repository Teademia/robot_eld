#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class LaserScanPublisher : public rclcpp::Node
{
public:
  LaserScanPublisher()
  : Node("laser_scan_publisher")
  {
    publisher_ = this->create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);
    timer_ = this->create_wall_timer(
      std::chrono::milliseconds(100), std::bind(&LaserScanPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    auto msg = sensor_msgs::msg::LaserScan();
    msg.header.stamp = this->now();
    msg.header.frame_id = "base_link";
    msg.angle_min = -M_PI;  // start angle of the scan [rad]
    msg.angle_max = M_PI;  // end angle of the scan [rad]
    msg.angle_increment = M_PI / 180.0;  // angular distance between measurements [rad]
    msg.time_increment = 0.0;  // time between measurements [seconds]
    msg.scan_time = 0.1;  // time between scans [seconds]
    msg.range_min = 0.1;  // minimum range value [m]
    msg.range_max = 10.0;  // maximum range value [m]

    // Generate ranges data
    int num_readings = std::ceil((msg.angle_max - msg.angle_min) / msg.angle_increment);
    msg.ranges.resize(num_readings, 1.0);  // all measurements set to 1 meter
    msg.intensities.resize(num_readings, 0.0);  // all intensities set to 0

    publisher_->publish(msg);
  }
  rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<LaserScanPublisher>());
  rclcpp::shutdown();
  return 0;
}