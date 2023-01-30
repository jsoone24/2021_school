#include "count_person.hpp"

namespace counter
{
    Counter::Counter()
    {
        pub = nh.advertise<count_person::message>("/person_num", 100);
        sub = nh.subscribe("/darknet_ros/bounding_boxes", 100, &Counter::MsgCallBack, this);
    }

    void Counter::MsgCallBack(const  darknet_ros_msgs::BoundingBoxes::ConstPtr &msg)
    {
        counter = 0;

        for(int i=0;i < msg->bounding_boxes.size(); i++)
        {
            if(msg->bounding_boxes[i].Class == "person")
                counter++;
        }

        my_data.count.data = counter;
        std::cout << "publish "<< my_data.count.data << std::endl;

        pub.publish(my_data);
    }
}
int main(int argc, char ** argv)
{
    ros::init(argc, argv, "count_person");
    counter::Counter C;

    ros::spin();

    return 0;
}