#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Int32.h>
#include <cstdlib>
#include <ctime>
#include <my_parameter_launch/class_msg.h>

namespace my_parameter_launch_class {
    class MyParameterLaunchClass {
    public:
        MyParameterLaunchClass();

        ~MyParameterLaunchClass();

        void run();

        int makeRandInt();

        float makeRandFloat();

        std::string makeString();

    private:
        ros::NodeHandle nh;
        ros::NodeHandle private_nh;

        ros::Publisher pub_rand_int_;
        ros::Publisher pub_class_msg_;

        std_msgs::Int32 my_data_int_;
        my_parameter_launch::class_msg class_msg_;

        //parameter using private_nh;
        int param_int_;
        float param_float_;
        std::string param_string_;

        //parameter using nh;
        int param_int_nh_;
        float param_float_nh_;
        std::string param_string_nh_;
    };
}