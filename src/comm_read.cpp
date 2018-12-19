#include "comm_read.h"

namespace serial_mul
{

comm_read::comm_read(std::string serial_port, int baud):dev(serial_port),serial_baudrate(baud)
{
  //std::string serial_port;
  data_len = sizeof(data_t);
  // dev = "/dev/ttyUSB1";
  // serial_baudrate = B9600;
}

bool comm_read::read_setup()
{
  // fd = open (dev.c_str(), O_RDWR);
  fd = open (dev.c_str(),O_RDWR|O_NOCTTY|O_NDELAY);
  sleep(1);
  if(fd==-1)
  {
    //perror("error!");
    return false;
  }
  struct termios oldtio;
  tcgetattr(fd,&oldtio);
  if(tcgetattr(fd,&oldtio)!=0)
  {
    //perror("error!");
    return false;
  }
  struct termios newtio;
  newtio.c_cflag|=CRTSCTS|CS8|CLOCAL|CREAD;
  newtio.c_cflag&=~PARODD;
  newtio.c_cflag&=~CSTOPB;
  // newtio.c_cflag=CLOCAL|CREAD;
  cfsetispeed(&newtio,serial_baudrate);
  newtio.c_iflag=IGNPAR;
  newtio.c_oflag=0;
  newtio.c_lflag=0;
  newtio.c_cc[VMIN]=1;
  // newtio.c_cc[VMIN]=0;
  newtio.c_cc[VTIME]=10;
  tcflush(fd,TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
  return true;
}



void comm_read::read_data(int32_t sec, int32_t nsec)
{
  
  uint8_t buff[data_len];
  memset(buff, 0, data_len);
  read(fd,buff,sizeof(data_t));
  memcpy(&data, buff, sizeof(data_t));
  if(data.sof==0xA5 && data.end == 0xFE)
  {
    pubCmdData.wr_sec = data.sec;
    pubCmdData.wr_nsec = data.nsec;
    pubCmdData.rd_sec = sec;
    pubCmdData.rd_nsec = nsec;
    pubCmdData.a_x = (double)data.ax;
    pubCmdData.a_y = (double)data.ay;
    pubCmdData.v_z = (double)data.vz;
    pubCmdData.yaw_rate = (double)data.z_rate;
    pubCmdData.a_x /=10000;
    pubCmdData.a_y /=10000;
    pubCmdData.v_z /=10000;
    pubCmdData.yaw_rate /=10000;
  }
}

comm_read::~comm_read()
{
  close(fd);
}

}//namespace serial_comm

