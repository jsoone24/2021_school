//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_MULTIPLE_TOPIC_SUBSCRIBE_HPP
#define SRC_MULTIPLE_TOPIC_SUBSCRIBE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Int32.h>
#include <my_publisher_class/class_msg.h>

namespace multiple_topic_subscribe {
    class MultipleTopicSubscribe {
    public:
        MultipleTopicSubscribe();

        ~MultipleTopicSubscribe();

        void callbackRandInt(const std_msgs::Int32::ConstPtr &msg);

        void callbackClassMsg(const my_publisher_class::class_msg::ConstPtr &msg);

        void run();

    private:
        ros::NodeHandle nh;

        ros::Subscriber sub_rand_int;
        ros::Subscriber sub_class_msg;

        int value_int_;
        float value_float_;
        std::string str_;
    };
}
#endif //SRC_MULTIPLE_TOPIC_SUBSCRIBE_HPP
