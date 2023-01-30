//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_PUB_SUBJECT_NODE_HPP
#define SRC_PUB_SUBJECT_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/String.h>
#include <cstdlib>
#include <ctime>

namespace pub_subject_class {
    class PubSubjectClass {
    public:
        PubSubjectClass();

        ~PubSubjectClass();

        void run();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_subject;

        std_msgs::String subject;
    };
}

#endif //SRC_PUB_SUBJECT_NODE_HPP
