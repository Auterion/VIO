#ifndef PX4_REALSENSE_BRIDGE
#define PX4_REALSENSE_BRIDGE

#include <nav_msgs/Odometry.h>
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

namespace bridge {

class PX4_Realsense_Bridge {
 public:
  PX4_Realsense_Bridge(const ros::NodeHandle& nh);
  ~PX4_Realsense_Bridge();

 private:
  ros::NodeHandle nh_;

  tf::TransformListener* tf_listener_;
  tf::TransformBroadcaster tf_broadcaster_;

  // Subscribers
  ros::Subscriber odom_sub_;
  // Publishers
  ros::Publisher mavros_odom_pub_;

  void odomCallback(const nav_msgs::Odometry& msg);
};
}
#endif  // PX4_REALSENSE_BRIDGE
