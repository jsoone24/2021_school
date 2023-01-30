//
// Created by jongsoo on 21. 3. 28..
//

#include "multiple_topic_subscribe_node.hpp"

namespace multiple_topic_subscribe {
    MultipleTopicSubscribe::MultipleTopicSubscribe() {
        sub_rand_int = nh.subscribe("/my_first_topic", 100, &MultipleTopicSubscribe::callbackRandInt, this);
        sub_class_msg = nh.subscribe("/my_first_msg", 100, &MultipleTopicSubscribe::callbackClassMsg, this);

        value_int_ = 0;
        value_float_ = 0.0;
        str_ = "";
    }

    MultipleTopicSubscribe::~MultipleTopicSubscribe() {

    }

    void MultipleTopicSubscribe::callbackRandInt(const std_msgs::Int32_<std::allocator<void>>::ConstPtr &msg) {
        //std::cout << "my_subscriber node subscribes [ " << msg->data << " ] " << std::endl;
        value_int_ = msg->data;
    }

    void MultipleTopicSubscribe::callbackClassMsg(const my_publisher_class::class_msg::ConstPtr &msg) {
        //std::cout << "my_subscriber node subscribes [ " << msg->float_data.data << " , " << msg->string_data.data << " ] " << std::endl;
        str_ = msg->string_data.data;
        value_float_ = msg->float_data.data;
    }

    void MultipleTopicSubscribe::run() {
        std::cout << "int     data : " << value_int_ << std::endl;
        std::cout << "float   data : " << value_float_ << std::endl;
        std::cout << "string  data : " << str_ << std::endl;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "multiple_topic_subscribe");
    multiple_topic_subscribe::MultipleTopicSubscribe MTS;
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        MTS.run();
        ros::spinOnce();
        loop_rate.sleep();
    }
}