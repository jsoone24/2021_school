//
// Created by jongsoo on 21. 4. 7..
//

#ifndef SRC_MY_DH_PARAMETERS_NODE_HPP
#define SRC_MY_DH_PARAMETERS_NODE_HPP

#include "ros/ros.h"
#include "tf/transform_broadcaster.h"

namespace my_dh_parameters
{
    class MYDhParameters
    {
    public:
        MYDhParameters();
        ~MYDhParameters();

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
#endif //SRC_MY_DH_PARAMETERS_NODE_HPP
