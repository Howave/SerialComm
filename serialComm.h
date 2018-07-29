// created to communication thre serial ports


#ifndef SERIALCOMMU_H
#define SERIALCOMMU_H

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<termios.h>
#include<errno.h>
#include<string.h>

#define FALSE  -1  
#define TRUE   0 

class SerialComm
{
	public:
		SerialComm();
	  	~SerialComm();
		int UART0_Send(char *send_buf, int data_len);

	private:
		int UART0_Open(int fd, char* port);
		int UART0_Init(int fd, int speed, int flow_ctrl, int databits,int stopbits,int parity);
		int UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);
		void UART0_Close(int fd);

		int fd_;
		int err;
		//char send_buf[100];

};


#endif // SERIALCOMMU_H
