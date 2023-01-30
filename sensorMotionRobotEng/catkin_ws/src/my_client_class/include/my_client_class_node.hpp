//
// Created by jongsoo on 21. 4. 3..
//

#ifndef SRC_MY_CLIENT_NODE_HPP
#define SRC_MY_CLIENT_NODE_HPP

#include "ros/ros.h"
#include "cstdlib"
#include "ctime"
#include "my_server_class/add_two_ints.h"

namespace my_client_class {
    class MyClientClass {
    public:
        MyClientClass();

        ~MyClientClass();

        void run();

        void setParam(int argc, char **argv);

    private:
        ros::NodeHandle nh;
        ros::ServiceClient client_;

        int argc_;
        char **argv_;
    };
}

#endif //SRC_MY_CLIENT_NODE_HPP
