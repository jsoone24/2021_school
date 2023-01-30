#include<ros/ros.h>
#include<nav_msgs/Odometry.h>
#include<tf/transform_broadcaster.h>
#include<string>
#include<cmath>
#include<gazebo_msgs/LinkStates.h>
#include<geometry_msgs/Twist.h>
#include<vector>
#include<iostream>

namespace mobile_robot_odometry
{
    class MobileRobotOdomety
    {
        public:
            MobileRobotOdomety();
            ~MobileRobotOdomety();

            void calcWheelVelocityGazeboCB(const gazebo_msgs::LinkStates::ConstPtr& ptr);
            void boardcastTransform();
            void pubTF();

        private:
            ros::NodeHandle nh;
            ros::NodeHandle private_nh;

            ros::Subscriber sub;
            ros::Publisher odomPub;

            std::string base_link_id, odom_link_id, wheel_1_id, wheel_3_id;
            double separation_length;
            double x_dot, y_dot, theta_dot;
            double x, y, theta;
            int seq;
            tf::TransformBroadcaster br;

            double velocity;
            double wheel_1_velocity;
            double wheel_3_velocity;
            ros::Time cur;

            double R = 1.0;
    };
}