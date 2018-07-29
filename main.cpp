#include <iostream>
#include <unistd.h>
#include "serialComm.h"

SerialComm serial_comm;

int set_relay_bitmat(char bitmap)
{
	char open_buf[128] = {0};
    char close_buf[128] = {0};

    open_buf[2] = 0xff;
    close_buf[2] = 0xff;

    //customer protocol 
    for (int i = 0; i < 8; i++)
    {
        if (((bitmap >> i) & 0x01) > 0) {
            open_buf[1] = 0xf0 | (i + 1);
            serial_comm.UART0_Send(open_buf, 3);
            usleep(100000);
        }
    }

    usleep(1000000);

    for (int i = 0; i < 8; i++)
    {
        if (((bitmap >> i) & 0x01) > 0) {
            open_buf[1] = (i + 1);
            serial_comm.UART0_Send(open_buf, 3);
            usleep(100000);
        }
    }

	return 0;
}

int set_relay_index(char relay_idx)
{
	char open_buf[128] = {0};
    char close_buf[128] = {0};

    if ((relay_idx < 1) || (relay_idx > 8))
        return -1;

    //customer protocol 
    open_buf[1] = 0xf0 | relay_idx;
    open_buf[2] = 0xff;

    close_buf[1] = relay_idx;
    close_buf[2] = 0xff;
    
    int len = serial_comm.UART0_Send(open_buf, 3);
    usleep(1000000);
    len = serial_comm.UART0_Send(close_buf, 3);

	return len;
}

int main() {

    SerialComm* serial_comm1 = new SerialComm();
    char send_buf[128] = {0};

    send_buf[1] = 0xf1;
    send_buf[2] = 0xff;
    int len = serial_comm1->UART0_Send(send_buf, 3);

    usleep(1000000);

    send_buf[1] = 0x01;
    send_buf[2] = 0xff;
    len = serial_comm1->UART0_Send(send_buf, 3);

    usleep(1000000);

    set_relay_index(8);

    usleep(1000000);

    set_relay_bitmat(0xff);

    delete serial_comm1;
    serial_comm1 = NULL;

    return 0;
}
