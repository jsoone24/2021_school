//
// Created by jongsoo on 21. 3. 28..
//

#include <random_gen_y_node.hpp>

namespace random_generator_y {

    RandomGeneratorY::RandomGeneratorY() {
        pub_random_y_ = nh.advertise<random_gen_x::control_msg>("/second", 100);
        sub_prev_msg_ = nh.subscribe<random_gen_x::control_msg>("/first", 100, &RandomGeneratorY::callbackPrevMsg, this);
    }

    RandomGeneratorY::~RandomGeneratorY() {

    }

    void RandomGeneratorY::callbackPrevMsg(const random_gen_x::control_msg::ConstPtr &ptr) {
        random_gen_x::control_msg cm;
        cm.rand_x.data = ptr->rand_x.data;
        cm.rand_y.data = genY();

        pub_random_y_.publish(cm);
    }

    float RandomGeneratorY::genY() {
        float result;

        result = (rand() % 300) / 100.0;
        return result;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "random_gen_y_node");
    random_generator_y::RandomGeneratorY RGY;
    ros::spin();
}