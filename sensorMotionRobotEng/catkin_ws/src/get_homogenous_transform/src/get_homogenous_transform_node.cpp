//
// Created by jongsoo on 21. 4. 14..
//

#include "ros/ros.h"
#include <eigen3/Eigen/Dense>
#include "iostream"

#define PI 3.141592

Eigen::Matrix4d RotZ(double rad) {
    Eigen::Matrix4d mat;

    mat << cos(rad), -sin(rad), 0, 0,
            sin(rad), cos(rad), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return mat;
}

Eigen::Matrix4d TransZ(double d) {
    Eigen::Matrix4d mat;

    mat << 1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, d,
            0, 0, 0, 1;

    return mat;
}

Eigen::Matrix4d TransX(double a) {
    Eigen::Matrix4d mat;

    mat << 1, 0, 0, a,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1;

    return mat;
}

Eigen::Matrix4d RotX(double rad) {
    Eigen::Matrix4d mat;

    mat << 1, 0, 0, 0,
            0, cos(rad), -sin(rad), 0,
            0, sin(rad), cos(rad), 0,
            0, 0, 0, 1;

    return mat;
}

double deg2rad(double degree) {
    return degree / 180.0 * PI;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "get_homogenous_transform_node");
    ros::NodeHandle nh;

    double theta_degree = 60.0;
    double theta_radian = deg2rad(theta_degree);

    double alpha_degree = 90.0;
    double alpha_radian = deg2rad(alpha_degree);

    Eigen::Matrix4d result = RotZ(theta_radian) * TransZ(1) * TransX(0) * RotX(alpha_radian);

    std::cout << result << std::endl;

    Eigen::Vector4d p(0, 0, 0, 1);
    Eigen::Vector4d transformed_p1;
    Eigen::Vector4d transformed_p2;

    transformed_p1 = result * p; // p : child TF origin, transformed_p1 : child origin's location on parent TF
    transformed_p2 = result.inverse() * p;// p : parent TF origin, transformed_p2 : parent origin's location on child TF

    std::cout << "child_link TF origin's position on parent_link TF" << std::endl;
    std::cout << "x : " << transformed_p1[0] << std::endl;
    std::cout << "y : " << transformed_p1[1] << std::endl;
    std::cout << "z : " << transformed_p1[2] << std::endl;

    std::cout << "parent_link TF origin's position on child_link TF" << std::endl;
    std::cout << "x : " << transformed_p2[0] << std::endl;
    std::cout << "y : " << transformed_p2[1] << std::endl;
    std::cout << "z : " << transformed_p2[2] << std::endl;

    return 0;
}