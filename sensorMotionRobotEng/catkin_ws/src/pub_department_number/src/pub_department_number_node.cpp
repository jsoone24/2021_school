//
// Created by jongsoo on 21. 3. 28..
//

#include "pub_department_number_node.hpp"

namespace pub_department_number_class {
    PubDepartmentNumberClass::PubDepartmentNumberClass() {
        pub_department_number = nh.advertise<std_msgs::String>("/my_department_number", 100);
    }

    PubDepartmentNumberClass::~PubDepartmentNumberClass() {
    }

    void PubDepartmentNumberClass::run() {

        department_number.data = " 20170404 ";
        pub_department_number.publish(department_number);

        std::cout << "pub_dapartment_number node publishes [ " << department_number.data << " ] " << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pub_department_number_node");
    pub_department_number_class::PubDepartmentNumberClass PDNC;
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        PDNC.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
}
