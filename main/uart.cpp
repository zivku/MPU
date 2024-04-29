#include <stdio.h>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "function.h"
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
#define BUF_SIZE 128
/*--------------UART參數設置--uart_transmit-----------
char* DEV_PATH -> UART路徑
speed_t baudrate -> B115200、B9600...
uint8_t tx_data[] -> 傳輸資料
int tx_len -> 傳輸資料長度
int&received_bytes -> 返回讀取byte數量
-----------------------------------------------------*/
uint8_t* uart_transmit(char* DEV_PATH, speed_t baudRate, int vtime, uint8_t tx_data[], int tx_len, int&received_bytes)
{
    int fd;
    int ret;
    received_bytes = 0;
    uint8_t* rx_data = new uint8_t[BUF_SIZE];
    struct termios options;
    /* open uart */
    fd = open(DEV_PATH, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        printf("ERROR open %s ret=%d\n\r", DEV_PATH, fd);
        return 0;
    }
    /* configure uart */
    tcgetattr(fd, &options);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cc[VTIME] = vtime; // read timeout 10*100ms
    options.c_cc[VMIN] = 0;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(ICRNL | IXON);
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    /* write uart */
    ret = write(fd, tx_data, tx_len);
    if (ret != tx_len) {
        printf("ERROR write ret=%d\n", ret);
    }
    while ((ret = read(fd, rx_data, BUF_SIZE - 1)) > 0) {
        received_bytes += ret;
    }
    close(fd);
    return rx_data;
        
    
    
    
    
}
uint8_t* uart_read(char* DEV_PATH, speed_t baudRate, int vtime, int&received_bytes)
{
    int fd;
    int ret;
    received_bytes = 0;
    uint8_t* rx_data = new uint8_t[BUF_SIZE];
    struct termios options;
    /* open uart */
    fd = open(DEV_PATH, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        printf("ERROR open %s ret=%d\n\r", DEV_PATH, fd);
        return 0;
    }
    /* configure uart */
    tcgetattr(fd, &options);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cc[VTIME] = vtime; // read timeout 10*100ms
    options.c_cc[VMIN] = 0;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(ICRNL | IXON);
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    while ((ret = read(fd, rx_data, BUF_SIZE - 1)) > 0) {
        received_bytes += ret;
    }
    close(fd);
    return rx_data;
        
    
    
    
    
}
int uart_write(char* DEV_PATH, speed_t baudRate, int vtime, uint8_t tx_data[], int tx_len)
{
    int fd;
    int ret;
    uint8_t* rx_data = new uint8_t[BUF_SIZE];
    struct termios options;
    /* open uart */
    fd = open(DEV_PATH, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        printf("ERROR open %s ret=%d\n\r", DEV_PATH, fd);
        return 0;
    }
    /* configure uart */
    tcgetattr(fd, &options);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cc[VTIME] = vtime; // read timeout 10*100ms
    options.c_cc[VMIN] = 0;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_oflag &= ~OPOST;
    options.c_iflag &= ~(ICRNL | IXON);
    cfsetispeed(&options, baudRate);
    cfsetospeed(&options, baudRate);
    options.c_cflag |= (CLOCAL | CREAD);
    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);

    /* write uart */
    ret = write(fd, tx_data, tx_len);
    if (ret != tx_len) {
        printf("ERROR write ret=%d\n", ret);
        return -1;
    }
    
    
    close(fd);
    return 1;
    
}