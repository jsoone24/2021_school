#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Int32.h>
#include <cstdlib>
#include <ctime>
#include <my_publisher_class/class_msg.h>

namespace my_publisher_class {
    class MyPublisherClass {
    public:
        MyPublisherClass();

        ~MyPublisherClass();

        void run();

        int makeRandInt();

        float makeRandFloat();

        std::string makeString();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_rand_int_;
        ros::Publisher pub_class_msg_;

        std_msgs::Int32 my_data_int_;
        my_publisher_class::class_msg class_msg_;
    };
}