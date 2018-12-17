# *serial_comm*

# Install the ch341 Driver:
1. delete the old driver
```
cd /lib/modules/$(uname -r)/kernel/drivers/usb/serial 
sudo rm ch341.ko
```
2. compile the new driver (CH341SER_LINUX)
```
cd ./CH341SER_LINUX
make
sudo make load
```
3. Set the module driver loaded when the system start:
```
sudo cp ch34x.ko /lib/modules/$(uname -r)/kernel/drivers/usb/serial
sudo depmod
```

# Change the serial device mode
## Temporary:
1. get the device ID, such as /dev/ttyUSB0
```
ls /dev/ | grep ttyUSB
```
2. set the mode
```
sudo chmod a+rw /dev/ttyUSB0
```
## Forever
1. creat the mode rule file
```
sudo vim -p etc/udev/rules.d/70-ttyUSB.rules
```
2. add these config parameters to the file
```
KERNEL=="ttyUSB*", OWNER="root", GROUP="root", MODE="0666" 
```
3. save the file. 
4. log out and in. Or reboot.

# clone this reposity to your workspace and build, set the serial port and baudrate to the launch file, start to work.
```
git clone https://github.com/guozhenglong/serial_comm.git
```

# Some attention
## about the ch341 driver file
In the 591 line of ch34x.c, I have commented the "wait_queue_t wait". 
Add the head fuile "#include <linux/sched/signal.h>" in the line 33.
If not, failure will happend when building the driver. 

## about the float data transmission
In the serial write node, I make a tricks transform float data to int data. That make process of decodeing the data more simply. 
`(int) = (double)*10000` 
In the serial read node,
`(double) = (int)/10000`

# welcomt to report your question about this reposity. Email: guozhenglong.cn@gmail.com

   
