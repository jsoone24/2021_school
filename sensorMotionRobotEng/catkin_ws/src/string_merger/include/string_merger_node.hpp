//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_STRING_MERGER_NODE_HPP
#define SRC_STRING_MERGER_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/String.h>
#include <cstdlib>
#include <ctime>

namespace string_merger_class {
    class StringMergerClass {
    public:
        StringMergerClass();

        ~StringMergerClass();

        void callbackMajorMsg(const std_msgs::String::ConstPtr &msg);

        void callbackSubjectMsg(const std_msgs::String::ConstPtr &msg);

        void callbackDepartmentNumberMsg(const std_msgs::String::ConstPtr &msg);

        void callbackNameMsg(const std_msgs::String::ConstPtr &msg);

        void run();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_string_merger;
        ros::Subscriber sub_major;
        ros::Subscriber sub_subject;
        ros::Subscriber sub_department_number;
        ros::Subscriber sub_name;

        std_msgs::String merge;
        std::string major;
        std::string subject;
        std::string department_number;
        std::string name;
    };
}

#endif //SRC_STRING_MERGER_NODE_HPP
