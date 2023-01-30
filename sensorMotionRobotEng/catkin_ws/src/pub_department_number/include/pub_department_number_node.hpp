//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_PUB_DEPARTMENT_NUMBER_NODE_HPP
#define SRC_PUB_DEPARTMENT_NUMBER_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/String.h>
#include <cstdlib>
#include <ctime>

namespace pub_department_number_class {
    class PubDepartmentNumberClass {
    public:
        PubDepartmentNumberClass();

        ~PubDepartmentNumberClass();

        void run();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_department_number;

        std_msgs::String department_number;
    };
}

#endif //SRC_PUB_DEPARTMENT_NUMBER_NODE_HPP
