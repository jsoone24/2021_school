#include <state_publisher.hpp>

namespace state_publisher
{
    StatePublisher::StatePublisher()
    {
        joint_pub = nh.advertise<sensor_msgs::JointState>("joint_states", 1);
        
        nh.setParam("joint1", -1.0);
        nh.setParam("joint2", -1.0);  
        nh.setParam("joint3", -1.0);
        nh.setParam("joint4", -1.0);
    }

    StatePublisher::~StatePublisher()
    {

    }

    void StatePublisher::run()
    {
        sensor_msgs::JointState joint_state;

        //update joint_state
        nh.getParam("joint1", joint_angle_1);
        nh.getParam("joint2", joint_angle_2);
        nh.getParam("joint3", joint_angle_3);
        nh.getParam("joint4", joint_angle_4);

        joint_state.header.stamp = ros::Time::now();
        joint_state.name.push_back("joint1");
        joint_state.position.push_back(joint_angle_1);
        joint_state.name.push_back("joint2");
        joint_state.position.push_back(joint_angle_2);
        joint_state.name.push_back("joint3");
        joint_state.position.push_back(joint_angle_3);
        joint_state.name.push_back("joint4");
        joint_state.position.push_back(joint_angle_4);

        joint_pub.publish(joint_state);
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "custom_joint_state_publisher");

    state_publisher::StatePublisher SP;

    ros::Rate loop_rate(10);

    while(ros::ok())
    {
        SP.run();
        loop_rate.sleep();
    }

    return 1;
}