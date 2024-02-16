#ifndef _SERIAL_
#define _SERIAL_
#include <termios.h>		//Used for UART
/* uart to cp*/
class serial
{
private:
    /* data */
    void reverse( char *str, int len);
    int intToStr(int x,  char *str, int d);
    struct termios options;
    int sig;
    int index;
    int uart_;
    int fp;
public:
    serial(unsigned int baud,char* port);
    ~serial();
    void write_char(char *);
    void write_int(int data);
    void send_data(unsigned char rate_x,unsigned char rate_y,unsigned char quality);
    int  ISuartReady();
};
#endif
