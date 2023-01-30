//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_RANDOM_GEN_YAW_NODE_HPP
#define SRC_RANDOM_GEN_YAW_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Float64.h>
#include <cstdlib>
#include <ctime>
#include <random_gen_x/control_msg.h>

namespace random_generator_yaw {
    class RandomGeneratorYaw {
    public:
        RandomGeneratorYaw();

        ~RandomGeneratorYaw();

        void callbackPrevMsg(const random_gen_x::control_msg::ConstPtr &ptr);

        float genYaw();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_random_yaw_;
        ros::Subscriber sub_prev_msg_;

        random_gen_x::control_msg prev_cm_;
    };
}

#endif //SRC_RANDOM_GEN_YAW_NODE_HPP
