//
// Created by jongsoo on 21. 4. 3..
//

#include "ros/ros.h"
#include "cstdlib"
#include "ctime"
#include "my_server/add_two_ints.h"

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_client_node");

    ros::NodeHandle nh;
    ros::ServiceClient client = nh.serviceClient<my_server::add_two_ints>("my_service_node");

    srand((unsigned int) time(NULL));

    my_server::add_two_ints ati;

    if (argc == 3) {
        std::cout << "user int version " << std::endl;
        ati.request.a = atoi((argv[1]));
        ati.request.b = atoi(argv[2]);

        if (client.call(ati)) {
            std::cout << "request add :" << ati.request.a << " + " << ati.request.b << std::endl;
            std::cout << "receive sum from server : " << ati.response.sum << std::endl;
        }
    } else {
        std::cout << "random int version " << std::endl;

        ati.request.a = rand() % 100;
        ati.request.b = rand() % 100;

        if (client.call(ati)) {
            std::cout << "request add :" << ati.request.a << " + " << ati.request.b << std::endl;
            std::cout << "receive sum from server : " << ati.response.sum << std::endl;
        }
    }
}