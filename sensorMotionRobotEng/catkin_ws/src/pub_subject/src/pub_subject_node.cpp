//
// Created by jongsoo on 21. 3. 28..
//

#include <pub_subject_node.hpp>

namespace pub_subject_class {
    PubSubjectClass::PubSubjectClass() {
        pub_subject = nh.advertise<std_msgs::String>("/my_subject", 100);
    }

    PubSubjectClass::~PubSubjectClass() {
    }

    void PubSubjectClass::run() {

        subject.data = " sensor_motion_robot_enginnering ";
        pub_subject.publish(subject);

        std::cout << "pub_subject node publishes [ " << subject.data << " ] " << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pub_subject_node");
    pub_subject_class::PubSubjectClass PSC;
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        PSC.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
}
