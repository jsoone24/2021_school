#include <calc_base2end_tf.hpp>

namespace calc_base2end_TF {
    CalcBase2endTF::CalcBase2endTF() {
        joint_state_sub_ = nh.subscribe("/joint_states", 1, &CalcBase2endTF::callbackJointStates, this);
    }

    CalcBase2endTF::~CalcBase2endTF() {

    }

    void CalcBase2endTF::callbackJointStates(const sensor_msgs::JointStatePtr &ptr) {
        Eigen::Matrix4d final_mat;
        final_mat << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

        // todo
        // get vector value
        std::cout << "-------- Joint information ---------"<<std::endl;
        std::cout << "shoulder_pan_joint\t" << ptr->position[0] << std::endl;    //shoulder_pan_joint ptr->position[0]
        std::cout << "shoulder_lift_joint\t" << ptr->position[1] << std::endl;   //shoulder_lift_joint ptr->position[1]
        std::cout << "elbow_joint\t\t" << ptr->position[2] << std::endl;           //elbow_joint ptr->position[2]
        std::cout << "wrist_1_joint\t\t" << ptr->position[3] << std::endl;         //wrist_1_joint ptr->position[3]
        std::cout << "wrist_2_joint\t\t" << ptr->position[4] << std::endl;         //wrist_2_joint ptr->position[4]
        std::cout << "wrist_3_joint\t\t" << ptr->position[5] << std::endl;         //wrist_3_joint ptr->position[5]

        //Homogeneous Matrix
        Eigen::Matrix4d homo_mat_0, homo_mat_1, homo_mat_2, homo_mat_3, homo_mat_4, homo_mat_5;

        homo_mat_0 = RotZ(ptr->position[0]) * TransZ(d_dh[0]) * TransX(a_dh[0]) * RotX(alpha_dh[0]);  //shoulder_pan_joint -> base
        homo_mat_1 = RotZ(ptr->position[1]) * TransZ(d_dh[1]) * TransX(a_dh[1]) * RotX(alpha_dh[1]);  //shoulder_lift_joint -> shoulder_pan_joint
        homo_mat_2 = RotZ(ptr->position[2]) * TransZ(d_dh[2]) * TransX(a_dh[2]) * RotX(alpha_dh[2]);  //elbow_joint -> shoulder_lift_joint
        homo_mat_3 = RotZ(ptr->position[3]) * TransZ(d_dh[3]) * TransX(a_dh[3]) * RotX(alpha_dh[3]);  //wrist_1_joint -> elbow_joint
        homo_mat_4 = RotZ(ptr->position[4]) * TransZ(d_dh[4]) * TransX(a_dh[4]) * RotX(alpha_dh[4]);  //wrist_2_joint -> wrist_1_joint
        homo_mat_5 = RotZ(ptr->position[5]) * TransZ(d_dh[5]) * TransX(a_dh[5]) * RotX(alpha_dh[5]);  //wrist_3_joint -> wrist_2_joint

        final_mat = homo_mat_0 * homo_mat_1 * homo_mat_2 * homo_mat_3 * homo_mat_4 * homo_mat_5;                    //wrist_3_joint -> base

        std::cout << "-------- Homogenous Matrix from end-effector to base using D-H parameters ---------" << std::endl;
        std::cout << final_mat << std::endl;

        std::cout << "-------- validation ---------" << std::endl;
        Eigen::Vector4d end_effector_origin(0, 0, 0, 1);
        Eigen::Vector4d transformed_origin_on_base;

        transformed_origin_on_base = final_mat * end_effector_origin;

        double x = transformed_origin_on_base[0];
        double y = transformed_origin_on_base[1];
        double z = transformed_origin_on_base[2];

        std::cout << "calc [ x, y, z ] : [ " << x << ", " << y << ", " << z << " ]" << std::endl << std::endl;
        // compare tool0 TF

    }

    Eigen::Matrix4d CalcBase2endTF::RotZ(double rad) {
        Eigen::Matrix4d mat;

        mat << cos(rad), -sin(rad), 0, 0,
                sin(rad), cos(rad), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

        return mat;
    }

    Eigen::Matrix4d CalcBase2endTF::TransZ(double d) {
        Eigen::Matrix4d mat;

        mat << 1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, d,
                0, 0, 0, 1;

        return mat;
    }

    Eigen::Matrix4d CalcBase2endTF::TransX(double a) {
        Eigen::Matrix4d mat;

        mat << 1, 0, 0, a,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1;

        return mat;
    }

    Eigen::Matrix4d CalcBase2endTF::RotX(double rad) {
        Eigen::Matrix4d mat;

        mat << 1, 0, 0, 0,
                0, cos(rad), -sin(rad), 0,
                0, sin(rad), cos(rad), 0,
                0, 0, 0, 1;

        return mat;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "calc_base2end_TF");

    calc_base2end_TF::CalcBase2endTF CBT;

    ros::spin();
}