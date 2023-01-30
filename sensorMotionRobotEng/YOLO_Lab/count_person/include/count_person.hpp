#include <iostream>
#include <ros/ros.h>
#include <count_person/message.h>
#include <darknet_ros_msgs/BoundingBoxes.h>

namespace counter
{
    class Counter
    {
        public:
            Counter();
            count_person::message my_data;
            int counter;

            ros::Publisher pub;
            ros::Subscriber sub;
            ros::NodeHandle nh;
            
            void MsgCallBack(const  darknet_ros_msgs::BoundingBoxes::ConstPtr &msg);
    };
}