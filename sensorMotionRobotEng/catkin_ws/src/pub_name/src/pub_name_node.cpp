//
// Created by jongsoo on 21. 3. 28..
//

#include <pub_name_node.hpp>

namespace pub_name_class {
    PubNameClass::PubNameClass() {
        pub_name = nh.advertise<std_msgs::String>("/my_name", 100);
    }

    PubNameClass::~PubNameClass() {
    }

    void PubNameClass::run() {

        name.data = " Han Jong soo ";
        pub_name.publish(name);

        std::cout << "pub_name node publishes [ " << name.data << " ] " << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "pub_name_node");
    pub_name_class::PubNameClass PNC;
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        PNC.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
}
