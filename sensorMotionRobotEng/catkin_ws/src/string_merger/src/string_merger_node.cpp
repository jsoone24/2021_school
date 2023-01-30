//
// Created by jongsoo on 21. 3. 28..
//
#include <string_merger_node.hpp>

namespace string_merger_class {
    StringMergerClass::StringMergerClass() {
        sub_major = nh.subscribe("/my_major", 100, &StringMergerClass::callbackMajorMsg, this);
        sub_subject = nh.subscribe("/my_subject", 100, &StringMergerClass::callbackSubjectMsg, this);
        sub_department_number = nh.subscribe("/my_department_number", 100, &StringMergerClass::callbackDepartmentNumberMsg, this);
        sub_name = nh.subscribe("/my_name", 100, &StringMergerClass::callbackNameMsg, this);
        pub_string_merger = nh.advertise<std_msgs::String>("/my_information", 100);
    }

    StringMergerClass::~StringMergerClass() {
    }

    void StringMergerClass::run() {
        std::string ss = major + "/" + subject + "/" + department_number + "/" + name;
        merge.data = ss;
        std::cout << ss << std::endl;
        pub_string_merger.publish(merge);
    }

    void StringMergerClass::callbackMajorMsg(const std_msgs::String::ConstPtr &msg) {
        major = msg->data;
    }

    void StringMergerClass::callbackSubjectMsg(const std_msgs::String::ConstPtr &msg) {
        subject = msg->data;
    }

    void StringMergerClass::callbackDepartmentNumberMsg(const std_msgs::String::ConstPtr &msg) {
        department_number = msg->data;
    }

    void StringMergerClass::callbackNameMsg(const std_msgs::String::ConstPtr &msg) {
        name = msg->data;
    }
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "string_merger_node");
    string_merger_class::StringMergerClass SMC;
    ros::Rate loop_rate(10);

    while (ros::ok()) {
        SMC.run();
        ros::spinOnce();
        loop_rate.sleep();
    }
}

