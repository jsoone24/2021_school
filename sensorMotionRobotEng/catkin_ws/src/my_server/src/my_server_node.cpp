//
// Created by jongsoo on 21. 4. 3..
//

#include "my_server_node.hpp"

#include "ros/ros.h"
#include "my_server/add_two_ints.h"

bool callbackAdd(my_server::add_two_ints::Request &req, my_server::add_two_ints::Response &res) {
    res.sum = req.a + req.b;
    std::cout << "request    : " << req.a << " , " << req.b << std::endl;
    std::cout << "send       : " << res.sum << std::endl;

    ROS_INFO("request : x = %d, y = %d", req.a, req.b);
    ROS_INFO("sending back response :[%d]", res.sum);

    return true;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_server_node");
    ros::NodeHandle nh;

    ros::ServiceServer service = nh.advertiseService("my_service_node", callbackAdd);
    std::cout << "Ready to add two ints. " << std::endl;

    ros::spin();

    return 0;
}
