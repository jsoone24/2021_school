#include <ros/ros.h>
#include <eigen3/Eigen/Dense>
#include <iostream>
#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include <sensor_msgs/JointState.h> //#include <rostopic type /joint_states>

#define N_Joint 6

// RotZ(z) Trans(d_dh) Trans(a_dh) RotX(alpha_dh) = joint_angle joint_distance link_length link_twist_angle
static constexpr double d_dh[N_Joint] = {0.089159, 0.00000, 0.00000, 0.10915, 0.09465, 0.0823};     // joint distance
static constexpr double a_dh[N_Joint] = {0.00000, -0.42500, -0.39225, 0.00000, 0.00000, 0.00000};   // link_length
static constexpr double alpha_dh[N_Joint] = {1.570796327, 0, 0, 1.570796327, -1.570796327, 0};      // link_twist_angle

namespace calc_base2end_TF {
    class CalcBase2endTF {
    public:
        CalcBase2endTF();

        ~CalcBase2endTF();

        Eigen::Matrix4d RotZ(double rad);

        Eigen::Matrix4d TransZ(double d);

        Eigen::Matrix4d TransX(double a);

        Eigen::Matrix4d RotX(double rad);

        void callbackJointStates(const sensor_msgs::JointStatePtr &ptr);

    private:
        ros::NodeHandle nh;

        ros::Subscriber joint_state_sub_;

    };
}