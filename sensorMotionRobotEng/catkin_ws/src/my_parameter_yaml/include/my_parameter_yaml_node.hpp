//
// Created by jongsoo on 21. 4. 3..
//

#ifndef SRC_MY_PARAMETER_YAML_NODE_HPP
#define SRC_MY_PARAMETER_YAML_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Int32.h>
#include <cstdlib>
#include <ctime>
#include <my_parameter_yaml/class_msg.h>

namespace my_parameter_yaml_class {
    class MyParameterYamlClass {
    public:
        MyParameterYamlClass();

        ~MyParameterYamlClass();

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
        my_parameter_yaml::class_msg class_msg_;

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

#endif //SRC_MY_PARAMETER_YAML_NODE_HPP
