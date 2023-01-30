//
// Created by jongsoo on 21. 4. 3..
//

#include "my_parameter_yaml_node.hpp"

namespace my_parameter_yaml_class {
    MyParameterYamlClass::MyParameterYamlClass() : private_nh("~") {
        //launch file parameter
        if (!private_nh.getParam("param_int", param_int_)) throw std::runtime_error("fail to get param_int");
        if (!private_nh.getParam("param_float", param_float_)) throw std::runtime_error("fail to get param_float");
        if (!private_nh.getParam("param_string", param_string_)) throw std::runtime_error("fail to get param_string");

        if (!nh.getParam("/my_parameter_yaml_node/param_int", param_int_nh_)) throw std::runtime_error("fail to get param_int");
        if (!nh.getParam("/my_parameter_yaml_node/param_float", param_float_nh_)) throw std::runtime_error("fail to get param_float");
        if (!nh.getParam("/my_parameter_yaml_node/param_string", param_string_nh_)) throw std::runtime_error("fail to get param_string");

        std::cout << "==== getParam using private_nh ==== " << std::endl;
        std::cout << param_int_ << " , " << param_float_ << " , " << param_string_ << std::endl;
        std::cout << "==== getParam using nh ==== " << std::endl;
        std::cout << param_int_nh_ << " , " << param_float_nh_ << " , " << param_string_nh_ << std::endl;
        std::cout << "==== finish setting parameter ==== " << std::endl;

        pub_rand_int_ = nh.advertise<std_msgs::Int32>("/my_first_topic", 100);
        pub_class_msg_ = nh.advertise<my_parameter_yaml::class_msg>("/my_first_msg", 100);

        srand((unsigned int) time(NULL));
    }

    MyParameterYamlClass::~MyParameterYamlClass() {
    }

    void MyParameterYamlClass::run() {
        //Int32
        // my_data_int_.data = makeRandInt();
        my_data_int_.data = param_int_; //parameter mode

        //float_data
        // class_mgs_.float_data.data = makeRandFloat();
        class_msg_.float_data.data = param_float_;

        //String_data
        // class_msg_.string_data.data = makeString();
        class_msg_.string_data.data = param_string_;

        pub_rand_int_.publish(my_data_int_);
        pub_class_msg_.publish(class_msg_);

        std::cout << "my_parameter node publishes [ " << my_data_int_.data << "] and [ " << class_msg_.float_data.data << " , " << class_msg_.string_data.data << " ] " << std::endl;
    }

    int MyParameterYamlClass::makeRandInt() {
        int rand_int;

        rand_int = rand() % 100;

        return rand_int;
    }

    float MyParameterYamlClass::makeRandFloat() {
        float rand_float;

        rand_float = (rand() % 100) / 7.0;

        return rand_float;
    }

    std::string MyParameterYamlClass::makeString() {
        std::string ss;

        ss = "Sensor Motion Robot Engineer";

        return ss;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_parameter_yaml_node");

    my_parameter_yaml_class::MyParameterYamlClass MPLC;

    ros::Rate loop_rate(10);

    while (ros::ok()) {
        MPLC.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
}