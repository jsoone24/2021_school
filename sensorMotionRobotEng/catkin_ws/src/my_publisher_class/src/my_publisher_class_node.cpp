#include "my_publisher_class_node.hpp"

namespace my_publisher_class {
    MyPublisherClass::MyPublisherClass() {
        pub_rand_int_ = nh.advertise<std_msgs::Int32>("/my_first_topic", 100);
        pub_class_msg_ = nh.advertise<my_publisher_class::class_msg>("/my_first_msg", 100);

        srand((unsigned int) time(NULL));
    }

    MyPublisherClass::~MyPublisherClass() {
    }

    void MyPublisherClass::run() {
        //Int32
        my_data_int_.data = makeRandInt();

        //float_data
        class_msg_.float_data.data = makeRandFloat();

        //String_data
        class_msg_.string_data.data = makeString();

        pub_rand_int_.publish(my_data_int_);
        pub_class_msg_.publish(class_msg_);

        std::cout << "my_publisher node publishes [ " << my_data_int_.data << "] and [ " << class_msg_.float_data.data << " , " << class_msg_.string_data.data << " ] " << std::endl;
    }

    int MyPublisherClass::makeRandInt() {
        int rand_int;

        rand_int = rand() % 100;

        return rand_int;
    }

    float MyPublisherClass::makeRandFloat() {
        float rand_float;

        rand_float = (rand() % 100) / 7.0;

        return rand_float;
    }

    std::string MyPublisherClass::makeString() {
        std::string ss;

        ss = "Sensor Motion Robot Engineer";

        return ss;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "my_publisher");

    my_publisher_class::MyPublisherClass MPC;

    ros::Rate loop_rate(10);

    while (ros::ok()) {
        MPC.run();

        ros::spinOnce();
        loop_rate.sleep();
    }
}