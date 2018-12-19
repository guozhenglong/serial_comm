#ifndef COMM_WRITE_H
#define COMM_WRITE_H

#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
// #include <std_msgs/String.h>
#include <serial_comm/Cmd_uav.h>
#include <fcntl.h>      /*file control lib*/
#include <termios.h>    /*PPSIX terminal*/
#include <errno.h>      /*error information*/
#include <stdlib.h>     /*standard lib*/
#include <unistd.h>     /*Unix standard func*/


namespace serial_mul
{
typedef struct
{
    uint8_t sof;
    int32_t sec;
    int32_t nsec;
    int32_t ax;
    int32_t ay;
    int32_t vz;
    int32_t z_rate;
    uint8_t end;

}__attribute__((packed)) data_t;

class comm_write{
public:
  comm_write(std::string serial_port, int baud);
  ~comm_write();
  /**
   * @brief Initialization the serial paramters for writing data
   * @return
   */
  bool write_setup();
  /**
   * @brief Writing data to serial
   */
  void write_data(data_t data_to_write);
  serial_comm::Cmd_uav pubCmdData;
private:
  data_t data;
  std::string dev;
  int serial_baudrate;
  // std_msgs::String pubData;
  int fd;
  int data_len;
};

}

#endif // COMM_WRITE_H
