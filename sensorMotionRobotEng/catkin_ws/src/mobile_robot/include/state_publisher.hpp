#include <string>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <tf/transform_broadcaster.h>

namespace state_publisher
{
    class StatePublisher
    {
        public:
            StatePublisher();
            ~StatePublisher();

            void run();

        private:
            ros::NodeHandle nh;
            ros::Publisher joint_pub;

            double joint_angle_1;
            double joint_angle_2;
            double joint_angle_3;
            double joint_angle_4;
    };
}