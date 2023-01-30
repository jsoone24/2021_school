//
// Created by jongsoo on 21. 4. 3..
//

#ifndef SRC_MY_SERVER_CLASS_NODE_HPP
#define SRC_MY_SERVER_CLASS_NODE_HPP

#include "ros/ros.h"
#include "my_server_class/add_two_ints.h"

namespace my_server_class {
    class MyServerClass {
    public:
        MyServerClass();

        ~MyServerClass();

        bool callbackAdd(my_server_class::add_two_ints::Request &req, my_server_class::add_two_ints::Response &res);

    private:
        ros::NodeHandle nh;
        ros::ServiceServer service_;
    };
}

#endif //SRC_MY_SERVER_CLASS_NODE_HPP
