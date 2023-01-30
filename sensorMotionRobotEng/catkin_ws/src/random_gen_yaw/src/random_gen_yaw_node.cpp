//
// Created by jongsoo on 21. 3. 28..
//

#include <random_gen_yaw_node.hpp>

namespace random_generator_yaw {

    RandomGeneratorYaw::RandomGeneratorYaw() {
        pub_random_yaw_ = nh.advertise<random_gen_x::control_msg>("/third", 100);
        sub_prev_msg_ = nh.subscribe<random_gen_x::control_msg>("/second", 100, &RandomGeneratorYaw::callbackPrevMsg, this);
    }

    RandomGeneratorYaw::~RandomGeneratorYaw() {

    }

    void RandomGeneratorYaw::callbackPrevMsg(const random_gen_x::control_msg::ConstPtr &ptr) {
        random_gen_x::control_msg cm;

        cm.rand_x.data = ptr->rand_x.data;
        cm.rand_y.data = ptr->rand_y.data;
        cm.rand_yaw.data = genYaw();

        pub_random_yaw_.publish(cm);
    }

    float RandomGeneratorYaw::genYaw() {
        float result;

        result = (rand() % 628) / 100.0; //0<result<628
        return result;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "random_gen_yaw_node");
    random_generator_yaw::RandomGeneratorYaw RGYaw;
    ros::spin();
}