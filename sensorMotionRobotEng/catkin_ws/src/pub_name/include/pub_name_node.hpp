//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_PUB_NAME_NODE_HPP
#define SRC_PUB_NAME_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/String.h>
#include <cstdlib>
#include <ctime>

namespace pub_name_class {
    class PubNameClass {
    public:
        PubNameClass();

        ~PubNameClass();

        void run();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_name;

        std_msgs::String name;
    };
}

#endif //SRC_PUB_NAME_NODE_HPP
