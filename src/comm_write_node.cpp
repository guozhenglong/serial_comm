#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include "comm_write.h"

serial_mul::data_t data;
int i;
void vel_callback(const geometry_msgs::Twist::ConstPtr& vel_data)
{
  std::cout<<*vel_data<<std::endl;
  // *10000   transform double to int, for serial transmission.
  data.ax = (int32_t)(vel_data->linear.x*10000);
  data.ay = (int32_t)(vel_data->linear.y*10000);
  data.vz = (int32_t)(vel_data->linear.z*10000);
  data.z_rate = (int32_t)(vel_data->angular.z*10000);
}

int main(int argc, char *argv[])
{
  ros::init(argc, argv, "serialWrite");
  ros::NodeHandle nh;
  ros::NodeHandle pnh("~");
  std::string serial_port, serial_port_default;
  serial_port_default="/dev/ttyUSB0";
  int baud;
  float Hz, Hz_default;
  Hz_default = 1.0;

  pnh.param("serial_port_name", serial_port, serial_port_default); 
  pnh.param("baudrate", baud, B9600); 
  pnh.param("Hz", Hz, Hz_default); 
  
  serial_mul::comm_write uav_comm_write(serial_port,baud);
  ros::Rate loopRate(Hz);
  i=0;
  bool success = uav_comm_write.comm_write::write_setup();
  
  if (success == false)
  {
    perror("Cannot open the serial, error!");
    return -1;
  }
  std::cout<<"open serial port:"<<success<<std::endl; 
  ros::Subscriber sub = nh.subscribe("cmd_vel",5,vel_callback);
  while(nh.ok())
  {
    uav_comm_write.write_data(data);
    std::cout<<i++<<std::endl;
    ros::spinOnce();
    loopRate.sleep();
  }
  return 0;
}
