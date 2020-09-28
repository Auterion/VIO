#include "../../include/PX4_realsense_bridge/PX4_realsense_bridge.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

namespace bridge {

PX4_Realsense_Bridge::PX4_Realsense_Bridge(const ros::NodeHandle& nh)
    : nh_(nh) {
  // initialize subscribers
  odom_sub_ = nh_.subscribe<const nav_msgs::Odometry&>(
      "/camera/odom/sample_throttled", 10, &PX4_Realsense_Bridge::odomCallback, this);
  // publishers
  mavros_pose_pub_ = 
      nh_.advertise<geometry_msgs::PoseStamped>("/mavros/vision_pose/pose", 10);
  mavros_system_status_pub_ =
      nh_.advertise<mavros_msgs::CompanionProcessStatus>("/mavros/companion_process/status", 1);

  last_callback_time = ros::Time::now();

  status_mutex_.reset(new std::mutex);
  worker_ = std::thread(&PX4_Realsense_Bridge::publishSystemStatus, this);
};

PX4_Realsense_Bridge::~PX4_Realsense_Bridge() { }

void PX4_Realsense_Bridge::odomCallback(const nav_msgs::Odometry& msg) {
  // stamp and frame id are needed for the mavros odometry msg for to be received by uorb topics
  geometry_msgs::PoseStamped output;
  output.header.stamp = ros::Time::now();
  output.header.frame_id = msg.header.frame_id;
  output.pose = msg.pose.pose;
  mavros_pose_pub_.publish(output);

  flag_first_pose_received = true;

  { // lock mutex
    std::lock_guard<std::mutex> status_guard(*(status_mutex_));

    last_system_status_ = system_status_;

    // check confidence in vision estimate by looking at covariance
    if( msg.pose.covariance[0] > 0.1 ) // low confidence -> reboot companion
    {
      system_status_ = MAV_STATE::MAV_STATE_FLIGHT_TERMINATION;
    }
    else if( msg.pose.covariance[0] == 0.1 ) // medium confidence
    {
      system_status_ = MAV_STATE::MAV_STATE_CRITICAL;
    }
    else if( msg.pose.covariance[0] == 0.01 ) // high confidence
    {
      system_status_ = MAV_STATE::MAV_STATE_ACTIVE;
    }
    else
    {
      ROS_WARN_STREAM("Unexpected vision sensor variance");
    }  

    // publish system status immediately if it changed
    if( last_system_status_ != system_status_ )
    {
      mavros_msgs::CompanionProcessStatus status_msg;

      status_msg.header.stamp = ros::Time::now();
      status_msg.component = 197;  // MAV_COMP_ID_VISUAL_INERTIAL_ODOMETRY

      status_msg.state = (int)system_status_;

      mavros_system_status_pub_.publish(status_msg);
    }  

  last_callback_time = ros::Time::now();
    
  }
}


void PX4_Realsense_Bridge::publishSystemStatus(){
  

  while(ros::ok()){
    
    ros::Duration(1).sleep();

    if(flag_first_pose_received == true) { // only send heartbeat if we receive pose estimates at all

      // check if we received an recent update
      // otherwise let the companion computer restart
      if( (ros::Time::now()-last_callback_time) > ros::Duration(0.5) ){
        ROS_WARN_STREAM("Stopped receiving data from T265");
        system_status_ = MAV_STATE::MAV_STATE_FLIGHT_TERMINATION;
      }

      mavros_msgs::CompanionProcessStatus status_msg;

      status_msg.header.stamp = ros::Time::now();
      status_msg.component = 197;  // MAV_COMP_ID_VISUAL_INERTIAL_ODOMETRY
    
      { // lock mutex
        std::lock_guard<std::mutex> status_guard(*(status_mutex_));

        status_msg.state = (int)system_status_;

        mavros_system_status_pub_.publish(status_msg);
      }
    }
  }

}

}
