#include "comm_write.h"


namespace serial_mul
{

comm_write::comm_write(std::string serial_port, int baud):dev(serial_port),serial_baudrate(baud)
{
  data_len = sizeof(data_t);
  data.sof = 0xA5;
  data.end = 0xFE;
  // dev = "/dev/ttyUSB0";
  // serial_baudrate = B9600;
}

bool comm_write::write_setup()
{
  
  // fd = open (dev.c_str(),O_RDWR);
  fd = open(dev.c_str(),O_RDWR|O_NDELAY);
  sleep(1);
  if(fd==-1)
  {
    perror("Cannot open the serial!");
    return false;
  }
  struct termios oldtio;
  tcgetattr(fd,&oldtio);
  tcgetattr(fd,&oldtio);
  struct termios newtio;
  // newtio.c_cflag|=CS8|CLOCAL|CREAD;
  // newtio.c_cflag&=~PARODD;
  // newtio.c_cflag&=~CSTOPB;
  newtio.c_cflag=CS8|CLOCAL|CREAD;
  cfsetispeed(&newtio,serial_baudrate);
  newtio.c_iflag=IGNPAR;
  newtio.c_oflag=0;
  newtio.c_lflag=0;
  // newtio.c_cc[VMIN]=0;
  newtio.c_cc[VMIN]=1;
  newtio.c_cc[VTIME]=0;
  tcflush(fd,TCIFLUSH);
  tcsetattr(fd,TCSANOW,&newtio);
  return true;
}

void comm_write::write_data(serial_mul::data_t data_to_write)
{
  data_to_write.sof = data.sof;
  data_to_write.end = data.end;
  uint8_t buff[data_len];
  memset(buff, 0, data_len);
  memcpy(buff, &data_to_write, sizeof(data_t));
  write(fd, buff, sizeof(data_t));
}

comm_write::~comm_write()
{
  close(fd);
}

}
