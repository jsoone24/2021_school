//
// Created by jongsoo on 21. 4. 23..
//

#include "ros/ros.h"
#include <geometry_msgs/Twist.h>

int main(int argc, char **argv)
{
    //Initialize the ROS system and become a node
    ros::init(argc, argv, "turtleCtrl");
    ros::NodeHandle nh;
    ros::Rate rate(10);

    float velocity = 2.0;
    nh.setParam("turtleVelocity", velocity); // 매개변수 초기설정 velocity가 2여서 2로 초기화.

    //Create a publisher object.
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000);

    while(ros::ok())
    {
        geometry_msgs::Twist msg;
        nh.getParam("turtleVelocity", velocity);    //velocity 값을 매개변수로부터 받은 값으로 변경한다
        msg.linear.x = velocity;
        msg.angular.z = 1.0;

        pub.publish(msg);
        ros::spinOnce();

        rate.sleep();
    }
}