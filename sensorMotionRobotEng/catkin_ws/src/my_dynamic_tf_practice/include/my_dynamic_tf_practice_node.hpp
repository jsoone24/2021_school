//
// Created by jongsoo on 21. 4. 7..
//

#ifndef SRC_MY_DYNAMIC_TF_PRACTICE_NODE_HPP
#define SRC_MY_DYNAMIC_TF_PRACTICE_NODE_HPP

#include "ros/ros.h"
#include "tf/transform_broadcaster.h"

namespace my_dynamic_tf_practice
{
    class MYDynamicTfPractice
    {
    public:
        MYDynamicTfPractice();
        ~MYDynamicTfPractice();

        void pubTF();

    private:
        ros::NodeHandle nh;
        ros::Time last;

        tf::TransformBroadcaster br;

        double pi_;
        double angular_velocity_;
        double current_theta_;
        double yaw_;
    };
}
#endif //SRC_MY_DYNAMIC_TF_PRACTICE_NODE_HPP
