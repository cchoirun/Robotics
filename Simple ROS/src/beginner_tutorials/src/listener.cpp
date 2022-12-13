#include <bits/stdc++.h>
#include "beginner_tutorials/Num.h"
#include "ros/ros.h"

// Fungsi memanggil subscriber
void callback1(const beginner_tutorials::Num::ConstPtr& msg) {
  ROS_INFO("Mendapatkan INFO X: %ld px", msg->Numx);
  ROS_INFO("Mendapatkan INFO Y: %ld px", msg->Numy);
  ROS_INFO("Mendapatkan INFO RADIUS: %f cm", msg->radius);
  ROS_INFO("Mendapatkan INFO VOLUME: %f cm^3", msg->volume);
  ROS_INFO("Mendapatkan INFO L.PERMUKAAN: %f cm^2", msg->perm);
  ROS_INFO(" ");
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;
  ros::Subscriber sub1 = nh.subscribe("chatter1", 1000, callback1);

  ros::spin();
  return 0;
}