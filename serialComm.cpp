#include "serialComm.h"

SerialComm::SerialComm()
{
  	char device[13] = "/dev/ttyUSB0";
	fd_ = UART0_Open(fd_, device);
	do{
		err = UART0_Init(fd_, 9600, 0, 8, 1, 'N');
		printf("Serial port %s Init\n", device);
	}while(FALSE == err || FALSE == fd_);

}

SerialComm::~SerialComm()
{
	UART0_Close(fd_);
}

void SerialComm::UART0_Close(int fd)
{
	close(fd);
}

int SerialComm::UART0_Open(int fd, char* port)
{
	fd = open( port, O_RDWR|O_NOCTTY|O_NDELAY);
	if (FALSE == fd)
	{
		perror("Can't Open Serial Port");
		return(FALSE);
	}
	if(fcntl(fd, F_SETFL, 0) < 0)
	{
		printf("fcntl failed!\n");
		return(FALSE);
	}
	else
	{
	  	fcntl(fd, F_SETFL,0);
	}

	if(0 == isatty(STDIN_FILENO))
	{
		printf("standard input is not a terminal device\n");
		return(FALSE);
	}
	else
	{
		//printf("isatty success!\n");
	}
	//printf("fd->open=%d\n",fd);
	return fd;
}

int SerialComm::UART0_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
	int i;
	int status;
	int speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};
	int name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};

	struct termios options;

	if( tcgetattr( fd,&options)  !=  0)
	{
		perror("SetupSerial 1");
		return(FALSE);
	}

	// set speed frequency
	for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
	{
		if  (speed == name_arr[i])
		{
			cfsetispeed(&options, speed_arr[i]);
			cfsetospeed(&options, speed_arr[i]);
		}
	}

	options.c_cflag |= CLOCAL;
	options.c_cflag |= CREAD;

	switch(flow_ctrl)
	{
		case 0 :  
			options.c_cflag &= ~CRTSCTS;
			break;
		case 1 :  
			options.c_cflag |= CRTSCTS;
			break;
		case 2 :  
			options.c_cflag |= IXON | IXOFF | IXANY;
			break;
	}

	options.c_cflag &= ~CSIZE;
	switch (databits)
	{
		case 5:
			options.c_cflag |= CS5;
			break;
		case 6:
			options.c_cflag |= CS6;
			break;
		case 7:
			options.c_cflag |= CS7;
			break;
		case 8:
			options.c_cflag |= CS8;
			break;
		default:
			fprintf(stderr,"Unsupported data size\n");
		return (FALSE);
	}

	switch (parity)
	{
	  	case 'n':
		case 'N':
			options.c_cflag &= ~PARENB;
			options.c_iflag &= ~INPCK;
			break;
		case 'o':
		case 'O':
			options.c_cflag |= (PARODD | PARENB);
			options.c_iflag |= INPCK;
			break;
		case 'e':
		case 'E':
			options.c_cflag |= PARENB;
			options.c_cflag &= ~PARODD;
			options.c_iflag |= INPCK;
			break;
		case 's':
		case 'S':
			options.c_cflag &= ~PARENB;
			options.c_cflag &= ~CSTOPB;
			break;
		default:
			fprintf(stderr,"Unsupported parity\n");
			return (FALSE);

	}

	switch (stopbits)
	{
		case 1:
			options.c_cflag &= ~CSTOPB; break;
		case 2:
			options.c_cflag |= CSTOPB; break;
		default:
			fprintf(stderr,"Unsupported stop bits\n");
		return (FALSE);
	}

	options.c_oflag &= ~OPOST;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_cc[VTIME] = 1;
	options.c_cc[VMIN] = 1;
	tcflush(fd,TCIFLUSH);

	if (tcsetattr(fd,TCSANOW,&options) != 0)
	{
		perror("com set error!\n");
		return (FALSE);
	}
	    
	return (TRUE);

}


int SerialComm::UART0_Init(int fd, int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
    return (UART0_Set(fd, speed, flow_ctrl , databits, stopbits, parity));
}

int SerialComm::UART0_Send(char *send_buf, int data_len)
{
	int len = 0;

	len = write(fd_, send_buf, data_len);
	if (len == data_len )
	{
		return len;
	}
	else
	{
		tcflush(fd_,TCOFLUSH);
		return FALSE;
	}
}

