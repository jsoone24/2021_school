//
// Created by jongsoo on 21. 3. 28..
//

#include "my_subscriber_class_node.hpp"

namespace my_subscriber_class {
    MySubscriberClass::MySubscriberClass() {
        sub_rand_int = nh.subscribe("/my_first_topic", 100, &MySubscriberClass::callbackRandInt, this);
        sub_class_msg = nh.subscribe("/my_first_msg", 100, &MySubscriberClass::callbackClassMsg, this);
    }

    MySubscriberClass::~MySubscriberClass() {

    }

    void MySubscriberClass::callbackRandInt(const std_msgs::Int32_<std::allocator<void>>::ConstPtr &msg) {
        std::cout << "my_subscriber node subscribes [ " << msg->data << " ] " << std::endl;
    }

    void MySubscriberClass::callbackClassMsg(const my_publisher_class::class_msg::ConstPtr &msg) {
        std::cout << "my_subscriber node subscribes [ " << msg->float_data.data << " , " << msg->string_data.data << " ] " << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_subscriber");
    my_subscriber_class::MySubscriberClass MSc;
    ros::spin();
}