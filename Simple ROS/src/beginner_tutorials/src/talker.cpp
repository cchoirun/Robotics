#include <cmath>
#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "beginner_tutorials/Num.h"
#include "ros/ros.h"
#define pixel_cm 37.795275
#define PHI 3.141592
using namespace std;
using namespace cv;
long int x_basic, y_basic;
double rad_basic, vol_basic, lperm_basic;
ros::Publisher pub1;

// Fungsi memanggil subscriber
void call_sub(const beginner_tutorials::Num::ConstPtr &msg) {
  long int x, y;
  double r, v, l;
  x = msg->Numx;
  y = msg->Numy;
  r = msg->radius;
  v = msg->volume;
  l = msg->perm;

  ROS_INFO("INFO X : %ld", x);
  ROS_INFO("INFO Y : %ld", y);
  ROS_INFO("INFO RADIUS : %lf", r);
  ROS_INFO("INFO VOLUME : %lf", v);
  ROS_INFO("INFO L.PERMUKAAN : %lf", l);
  ROS_INFO(" ");
}

// Fungsi memanggil timer
void call_time(const ros::TimerEvent &e) {
  beginner_tutorials::Num data1;
  ros::Rate loop_rate(2);
  while (ros::ok()) {
    data1.Numx = x_basic;
    data1.Numy = y_basic;
    data1.radius = rad_basic;
    data1.volume = vol_basic;
    data1.perm = lperm_basic;

    pub1.publish(data1);

    ros::spinOnce();
    loop_rate.sleep();
  }
}

int main(int argc, char **argv) {
  // Area OpenCV : Mencari x,y, dan radius bola;
  vector<vector<Point>> contours;
  vector<Vec4i> hierarchy;

  Mat imgOut, imgDia, imgCanny, imgEro, abu, baca, baca1;
  Point tgh(0, 0);
  double jari;

  String gambar = "/home/cchoirun/Magang_ROS/src/beginner_tutorials/bola_futsal.jpg";
  baca1 = imread(gambar);
  resize(baca1, baca, Size(720, 720), INTER_LINEAR);
  Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
  cvtColor(baca, abu, COLOR_BGR2GRAY);
  Canny(abu, imgCanny, 70, 150);
  dilate(imgCanny, imgDia, kernel);

  findContours(imgDia, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
  vector<Rect> boundRect(contours.size());
  for (int i = 0; i < contours.size(); i++) {
    RotatedRect y = minAreaRect(contours[i]);

    if (contourArea(contours[i]) > 166000) {
      drawContours(baca, contours, -1, Scalar(0, 0, 255), 2, LINE_8, hierarchy,
                   1);
      boundRect[i] = boundingRect(contours[i]);
      tgh = y.center;
      jari = boundRect[i].width / 2;
    }
  }

  x_basic = tgh.x;
  y_basic = tgh.y;
  rad_basic = jari / pixel_cm;
  vol_basic = (4 / 3) * PHI * pow(rad_basic, 3);
  lperm_basic = 4 * PHI * pow(rad_basic, 2);

  // Area ROS : Melakukan Publisher
  ros::init(argc, argv, "talker");
  ros::NodeHandle nh;
  ros::MultiThreadedSpinner mts;
  pub1 = nh.advertise<beginner_tutorials::Num>("chatter1", 1000);
  ros::Subscriber sub = nh.subscribe("chatter1", 1000, call_sub);
  ros::Timer timer1 = nh.createTimer(ros::Duration(0.01), call_time);

  mts.spin();
}
