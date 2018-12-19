#ifndef COMMREAD_H
#define COMMREAD_H

#include <serial_comm/Cmd_uav.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <memory>
#include <vector>
#include <string>
#include <math.h>
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

class comm_read{
public:
  comm_read(std::string serial_port, int baud);
  ~comm_read();
  /**
   * @brief Initialization the serial paramters for reading data
   * @return
   */
  bool read_setup();

  /**
   * @brief reading data from serial
   */
  void read_data(int32_t sec, int32_t nsec);
public:
  serial_comm::Cmd_uav pubCmdData;
  
private:
  std::string dev;
  int serial_baudrate;
  int fd;
  int data_len;
  data_t data;
};

}


#endif // COMMREAD_H
