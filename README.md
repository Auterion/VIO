# VIO (Visual Inertial Odometry)


This package is a bridge from PX4 to the Realsense T265 camera, which provides odometry data.

## Dependencies
* ROS Kinetic (Ubuntu 16.04) or ROS Melodic (Ubuntu 18.04): [http://wiki.ros.org/ROS/Installation](http://wiki.ros.org/ROS/Installation)
* librealsense: [https://github.com/IntelRealSense/librealsense](https://github.com/IntelRealSense/librealsense)


## Installation
These steps contain the installation process, software dependencies and building instructions.

1. This is a ROS package, it assumes you have either ROS Kinetic (Ubuntu 16.04) or ROS Melodic (Ubuntu 18.04) installed, instructions can be found [here](http://wiki.ros.org/ROS/Installation). Please also make sure you install librealsense from [here](https://github.com/IntelRealSense/librealsense).

1. Install catkin and create your catkin workspace directory.

   ```bash
   sudo apt install python-catkin-tools
   mkdir -p ~/catkin_ws/src
   ```

1. Clone this repository in your catkin workspace.

   ```bash
   cd ~/catkin_ws/src
   git clone https://github.com/Auterion/VIO.git
   ```

1. Install MAVROS (version 0.29.0 or above).
   > **Note:** Instructions to install MAVROS from sources can be found [here](https://dev.px4.io/en/ros/mavros_installation.html).
   
   * Melodic
     ```bash
     sudo apt install ros-melodic-mavros ros-melodic-mavros-extras
     ```
   * Kinetic
     ```bash
     sudo apt install ros-kinetic-mavros ros-kinetic-mavros-extras
     ```

1. Build the package:

   ```bash
   cd ~/catkin_ws/src
   catkin build px4_realsense_bridge
   ```

1. Run th ROS node:

   ```bash
   cd ~/catkin_ws/src
   roslaunch px4_realsense_bridge bridge_mavros.launch
   ```

  > **Note** This launch file starts the mavros node as well. Mavros needs to be running but if it is started elsewhere the basic launch file *bridge.launch* can be used. The file *bridge_mavros_sitl.launch* is only for using this node in combination with the PX4 SITL toolchain (Simulation). Instructions on how to setup the PX4 SITL toolchain can be found [here](http://dev.px4.io/en/setup/dev_env_linux_ubuntu.html#common-dependencies). 
