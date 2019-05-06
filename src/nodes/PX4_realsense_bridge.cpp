#include "../../include/PX4_realsense_bridge/PX4_realsense_bridge.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

namespace bridge {

PX4_Realsense_Bridge::PX4_Realsense_Bridge(const ros::NodeHandle& nh)
    : nh_(nh) {
  tf_listener_ = new tf::TransformListener(
      ros::Duration(tf::Transformer::DEFAULT_CACHE_TIME), true);

  // initialize subscribers
  odom_sub_ = nh_.subscribe<const nav_msgs::Odometry&>(
      "/camera/odom/sample", 1, &PX4_Realsense_Bridge::odomCallback, this);
  // publishers
  mavros_odom_pub_ =
      nh_.advertise<nav_msgs::Odometry>("/mavros/odometry/out", 10);
};

PX4_Realsense_Bridge::~PX4_Realsense_Bridge() { delete tf_listener_; }


void PX4_Realsense_Bridge::odomCallback(const nav_msgs::Odometry& msg) {
  nav_msgs::Odometry output = msg;
  output.header.frame_id = "local_origin";
  output.child_frame_id = "camera_downward";
  mavros_odom_pub_.publish(output);
}
}
