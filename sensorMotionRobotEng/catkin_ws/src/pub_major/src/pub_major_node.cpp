//
// Created by jongsoo on 21. 3. 28..
//

#include <pub_major_node.hpp>

namespace pub_major_class {
    PubMajorClass::PubMajorClass() {
        pub_major = nh.advertise<std_msgs::String>("/my_major", 100);
    }

    PubMajorClass::~PubMajorClass() {
    }

    void PubMajorClass::run() {

        major.data = " smart system software ";
        pub_major.publish(major);

        std::cout << "pub_major node publishes [ " << major.data << " ] " << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pub_major_node");
    pub_major_class::PubMajorClass PMC;
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        PMC.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
}
