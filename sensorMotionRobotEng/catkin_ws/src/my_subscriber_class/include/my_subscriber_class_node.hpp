//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_MY_SUBSCRIBER_CLASS_HPP
#define SRC_MY_SUBSCRIBER_CLASS_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Int32.h>
#include <my_publisher_class/class_msg.h>

namespace my_subscriber_class {
    class MySubscriberClass {
    public:
        MySubscriberClass();

        ~MySubscriberClass();

        void callbackRandInt(const std_msgs::Int32::ConstPtr &msg);

        void callbackClassMsg(const my_publisher_class::class_msg::ConstPtr &msg);

    private:
        ros::NodeHandle nh;

        ros::Subscriber sub_rand_int;
        ros::Subscriber sub_class_msg;
    };
}
#endif //SRC_MY_SUBSCRIBER_CLASS_HPP
