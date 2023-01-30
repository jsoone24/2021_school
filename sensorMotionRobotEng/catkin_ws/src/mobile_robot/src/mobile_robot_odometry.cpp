#include <mobile_robot_odometry.hpp>

namespace mobile_robot_odometry
{
    MobileRobotOdomety::MobileRobotOdomety() : private_nh("~"), seq(0), x(0.0), y(0.0), theta(0.0)
    {
        sub = nh.subscribe("/gazebo/link_states", 100, &MobileRobotOdomety::calcWheelVelocityGazeboCB, this);
        odomPub = nh.advertise<nav_msgs::Odometry>("/custom_odom", 100);

        if (!private_nh.getParam("base_link_id", base_link_id))
            throw std::runtime_error("set base_link_id");
        if (!private_nh.getParam("odom_link_id", odom_link_id))
            throw std::runtime_error("set odom_link_id");
        if (!private_nh.getParam("leftwheel_linkname", wheel_1_id))
            throw std::runtime_error("set wheel_1_id");
        if (!private_nh.getParam("rightwheel_linkname", wheel_3_id))
            throw std::runtime_error("set wheel_3_id");
        if (!private_nh.getParam("separation_length", separation_length))
            throw std::runtime_error("set separation_length");

        cur = ros::Time::now();
    }

    MobileRobotOdomety::~MobileRobotOdomety()
    {
    }

    void MobileRobotOdomety::calcWheelVelocityGazeboCB(const gazebo_msgs::LinkStates::ConstPtr &ptr)
    {
        // 속도 계산 (v 를 구한다)
        wheel_1_velocity = sqrt(pow(ptr->twist[2].linear.x, 2) + pow(ptr->twist[2].linear.y, 2) + pow(ptr->twist[2].linear.z, 2));
        wheel_3_velocity = sqrt(pow(ptr->twist[4].linear.x, 2) + pow(ptr->twist[4].linear.y, 2) + pow(ptr->twist[4].linear.z, 2));

        velocity = R * (wheel_3_velocity + wheel_1_velocity) / 2;
        theta_dot = R * (wheel_3_velocity - wheel_1_velocity) / separation_length;
    }

    void MobileRobotOdomety::boardcastTransform()
    {

        tf::Transform transform;
        transform.setOrigin(tf::Vector3(x, y, 0.0));
        tf::Quaternion q;
        q.setRPY(0, 0, theta);
        transform.setRotation(q);

        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), odom_link_id, base_link_id)); // linking base_link
    }

    void MobileRobotOdomety::pubTF()
    {
        nav_msgs::Odometry odom;

        // todo - odometry 계산. ( theta, x_dot, y_dot, x, y )
        double dt = (ros::Time::now() - cur).toSec();
        theta += theta_dot * dt;
        x_dot = velocity * std::cos(theta);
        y_dot = velocity * std::sin(theta);
        x += x_dot * dt;
        y += y_dot * dt;

        odom.header.seq = seq++;
        odom.header.stamp = cur;
        odom.header.frame_id = odom_link_id;
        odom.child_frame_id = base_link_id;
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0;
        odom.pose.pose.orientation = tf::createQuaternionMsgFromYaw(theta);
        odom.twist.twist.linear.x = x_dot;
        odom.twist.twist.linear.y = y_dot;
        odom.twist.twist.angular.z = theta_dot;

        odomPub.publish(odom);
        boardcastTransform();
        cur = ros::Time::now();
    }
};

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "mobile_robot_odometry");
    mobile_robot_odometry::MobileRobotOdomety MRO;
    ros::Rate loop_rate(100); // dt is always 100ms

    while (ros::ok())
    {
        ros::spinOnce();
        MRO.pubTF();
        loop_rate.sleep();
    }
    return 0;
}
