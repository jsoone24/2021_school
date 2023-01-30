#include <ros/ros.h>
#include <eigen3/Eigen/Dense>
#include <iostream>

int main(int argc, char **argv)
{
        ros::init(argc, argv, "eigen_practice_node");
        ros::NodeHandle nh;

        Eigen::Matrix2d mat1;       // 2*2 matrix, double
        Eigen::Matrix3d mat2;       // 3*3 matrix, double
        Eigen::MatrixXd mat3(2, 5); // 4*4 matrix, double

        // Insert case 1
        mat1(0, 0) = 3.0;
        mat1(0, 1) = 7.0;
        mat1(1, 0) = mat1(0, 0) + mat1(0, 1);
        mat1(1, 1) = mat1(0, 0) - mat1(0, 1);

        std::cout << "make mat1" << std::endl;
        std::cout << mat1 << std::endl;

        // Insert case 2
        mat2 << 1, 2, 3,
            4, 0, 6,
            7, 0, 9;

        std::cout << "make mat2" << std::endl;
        std::cout << mat2 << std::endl;

        // Matrix Xd
        std::cout << "matrix mat3 size : " << mat3.rows() << " X " << mat3.cols() << std::endl;
        std::cout << "resize size!" << std::endl;
        mat3.resize(4, 3);
        std::cout << "resized matrix mat3 size : " << mat3.rows() << " X " << mat3.cols() << std::endl;

        // Inverse
        std::cout << "---- mat1 inverse ----" << std::endl;
        std::cout << mat1 << std::endl << std::endl;
        std::cout << mat1.inverse() << std::endl << std::endl;
        std::cout << mat1 * mat1.inverse() << std::endl;

        std::cout << "---- mat2 inverse ----" << std::endl;
        std::cout << mat2 << std::endl << std::endl;
        std::cout << mat2.inverse() << std::endl << std::endl;
        std::cout << mat2 * mat2.inverse() << std::endl;

        return 0;
}

//처음 ~parameter 가 시험범위 TF 안나옴