#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BAUDRATE B9600
#define IR_DEVICE "/dev/ttyUSB0"
#define FALSE 0
#define TRUE 1
#define BUFFER_BYTE_SIZE    150

#define METER_ID_BYTE_LEN   14

volatile int STOP=FALSE;

const char CURRENT_POWER[] = "1-0:1.7.0*255(";
const char METER_READING[] = "1-0:1.8.0*255(";

void checkCurrentPower(char* line)
{
    char* cEndPtr = NULL;
    float fPower=0, fMeterReading=0;

    if (0 == memcmp(CURRENT_POWER, line, METER_ID_BYTE_LEN))
    {
        fPower = strtof(&line[METER_ID_BYTE_LEN], &cEndPtr);
        printf("current power %f\n", fPower);
    }

    if (0 == memcmp(METER_READING, line, METER_ID_BYTE_LEN))
    {
        fMeterReading = strtof(&line[METER_ID_BYTE_LEN], &cEndPtr);
        printf("meter reading %f\n", fMeterReading);
    }
}

void main(void)
{
    int fd, res;
    struct termios oldtio,newtio;
    char cLine[BUFFER_BYTE_SIZE];

    fd = open(IR_DEVICE, O_RDONLY | O_NOCTTY );
    if (fd <0)
    {
        perror(IR_DEVICE); exit(-1);
    }

    tcgetattr(fd,&oldtio); /* save current port settings */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE |  CS7 | CLOCAL | CREAD | PARODD | CRTSCTS;
    newtio.c_iflag = BRKINT;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = ICANON;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd,TCSANOW,&newtio);
    while (STOP==FALSE)
    {
        /* clear input buffer */
        memset(cLine, 0, BUFFER_BYTE_SIZE);

        /* read line */
        res = read(fd,cLine,BUFFER_BYTE_SIZE);

        if (-1 == res)
        {
            continue;
        }

        checkCurrentPower(cLine);

        /* check for end of meter block */
        if (cLine[0]=='!')
        {
            STOP=TRUE;
        }
    }

    tcsetattr(fd,TCSANOW,&oldtio);

    close(fd);
}
