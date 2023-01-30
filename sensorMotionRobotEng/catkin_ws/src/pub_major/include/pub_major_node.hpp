//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_PUB_MAJOR_NODE_HPP
#define SRC_PUB_MAJOR_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/String.h>
#include <cstdlib>
#include <ctime>

namespace pub_major_class {
    class PubMajorClass {
    public:
        PubMajorClass();

        ~PubMajorClass();

        void run();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_major;

        std_msgs::String major;
    };
}

#endif //SRC_PUB_MAJOR_NODE_HPP
