//
// Created by jongsoo on 21. 4. 7..
//
#include "my_dynamic_tf_practice_node.hpp"

namespace my_dynamic_tf_practice {
    MYDynamicTfPractice::MYDynamicTfPractice() {
        angular_velocity_ = 2.0;
        current_theta_ = 0.0;
        yaw_ = 0.0;
    }

    MYDynamicTfPractice::~MYDynamicTfPractice() {

    }

    void MYDynamicTfPractice::pubTF() {
        ros::Time now = ros::Time::now();
        double dt = (now - last).toSec();

        current_theta_ += angular_velocity_ * dt;
        last = now;

        //calculate child frame - translation
        double x = cos(current_theta_);
        double y = sin(current_theta_);

        //calculate child frame - rotation
        yaw_ += 2.0 * dt;

        //generate tf msg
        tf::Transform transform;
        transform.setOrigin(tf::Vector3(x, y, 0.0));
        tf::Quaternion q;
        q.setRPY(0, 0, yaw_);
        transform.setRotation(q);

        //publish tf
        br.sendTransform(tf::StampedTransform(transform, now, "parent_link", "child_link"));
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_dynamic_tf_practice");

    my_dynamic_tf_practice::MYDynamicTfPractice MDTP;

    ros::Rate loop_rate(100);

    while (ros::ok()) {
        MDTP.pubTF();
        loop_rate.sleep();
    }

    return 0;
}