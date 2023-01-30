#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Int32.h>
#include <cstdlib>
#include <ctime>
#include <my_publisher/my_msg.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_publisher");
    ros::NodeHandle nh;
    ros::Publisher pub_rand_int = nh.advertise<std_msgs::Int32>("/my_first_topic", 100);
    ros::Publisher pub_my_msg = nh.advertise<my_publisher::my_msg>("/my_first_msg", 100);
    ros::Rate loop_rate(10);

    srand((unsigned int) time(NULL));

    while (ros::ok()) {
        //my_msg
        my_publisher::my_msg mm;
        mm.float_data.data = (rand() % 100) / 7.0;

        std::stringstream ss;
        ss << "I like apple";
        mm.string_data.data = ss.str();

        //Int32
        std_msgs::Int32 my_data;
        my_data.data = rand() % 100;

        pub_rand_int.publish(my_data);
        pub_my_msg.publish(mm);

        std::cout << "my_publisher node publishes [ " << my_data.data << " ] and [ "
                  << mm.float_data.data << " , " << mm.string_data.data << " ] " << std::endl;

        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}