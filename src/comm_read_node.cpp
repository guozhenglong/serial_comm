#include "ros/ros.h"
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include "comm_read.h"

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "serialRead");
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  std::string serial_port, serial_port_default;
  serial_port_default="/dev/ttyUSB0";
  int baud;
  float Hz,Hz_default;
  Hz_default = 1.0;
  int i=0;

  pnh.param("serial_port_name", serial_port, serial_port_default); 
  pnh.param("baudrate", baud, B9600); 
  pnh.param("Hz",Hz,Hz_default);
  ros::Publisher publisher = nh.advertise<geometry_msgs::Twist>("uav_cmd_vel", 100);
  ros::Publisher raw_data_pub = nh.advertise<serial_comm::Cmd_uav>("Cmd_uav_raw", 100);
  serial_mul::comm_read uav_comm_read(serial_port,baud);
  geometry_msgs::Twist pub_uav_cmd;
  ros::Rate loopRate(Hz);

  //set up the serial
  bool sucess = uav_comm_read.comm_read::read_setup();
  if (sucess == false)
  {
    perror("error!");
    return -1;
  }
  

  while(nh.ok())
  {
    ros::spinOnce();
    i++;
    uav_comm_read.read_data();
    pub_uav_cmd.linear.x = uav_comm_read.pubCmdData.a_x;
    pub_uav_cmd.linear.y = uav_comm_read.pubCmdData.a_y;
    pub_uav_cmd.linear.z = uav_comm_read.pubCmdData.v_z;
    pub_uav_cmd.angular.z = uav_comm_read.pubCmdData.yaw_rate;

    std::cout<<i<<std::endl;
    printf("cmd ax:  %f\n",pub_uav_cmd.linear.x);
    printf("cmd ay:  %f\n",pub_uav_cmd.linear.y);
    printf("cmd vz: %f\n",pub_uav_cmd.linear.z);
    printf("yaw rate:    %f\n",pub_uav_cmd.angular.z);
    printf("---------------------\n");
    publisher.publish(pub_uav_cmd);
    raw_data_pub.publish(uav_comm_read.pubCmdData);
    loopRate.sleep();
  }
  return 0;
}
