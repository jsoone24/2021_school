//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_RANDOM_GEN_X_NODE_HPP
#define SRC_RANDOM_GEN_X_NODE_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Float64.h>
#include <cstdlib>
#include <ctime>
#include <random_gen_x/control_msg.h>

namespace random_generator_x {
    class RandomGeneratorX {
    public:
        RandomGeneratorX();

        ~RandomGeneratorX();

        void run();

        float genX();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_random_x_;
    };
}
#endif //SRC_RANDOM_GEN_X_NODE_HPP
