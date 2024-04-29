#include <stdint.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <fcntl.h>
#include "function.h"
#include <thread>
char GPIO_CHIP0[] = "/dev/gpiochip0";
//#include "shared_sync.h"
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
std::mutex color_data_buf_mutex; 
std::condition_variable color_data_ready;
bool color_data_changed;
/*--------------SPI參數設置--spi_transmit-------------
char* DEV_SPI -> SPI路徑
int spi_speed -> SPI頻率設置
uint8_t data_buf[] -> 傳輸資料
int data_len -> 傳輸資料長度
int state -> 傳輸模式, 1為連續傳輸 ,0為單筆傳輸
-----------------------------------------------------*/
void spi_transmit(char* DEV_PATH, int spi_speed, uint8_t data_buf[], int data_len, int state)
{
    int fd;
    int ret;
    unsigned int mode, speed;
    struct spi_ioc_transfer xfer[2] = {0};

    // open device node
    fd = open(DEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("ERROR open %s ret=%d\n", DEV_PATH, fd);
    }

    // set spi mode
    mode = SPI_MODE_0;
    if (ioctl(fd, SPI_IOC_WR_MODE32, &mode) < 0) {
        printf("ERROR ioctl() set mode\n");
    }
    if (ioctl(fd, SPI_IOC_RD_MODE32, &ret) < 0) {
        printf("ERROR ioctl() get mode\n");
    } else
        printf("mode set to %d\n", (unsigned int)ret);

    // set spi speed
    speed = spi_speed;
    //speed = 3333333;
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        printf("ERROR ioctl() set speed\n");
    }
    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &ret) < 0) {
        printf("ERROR ioctl() get speed\n");
    } else
        printf("speed set to %d\n", ret);
    
    // transfer data
    if (state == 1) {
        while (1) {
            std::unique_lock<std::mutex> lock(color_data_buf_mutex);
            xfer[0].len = data_len;
            xfer[0].tx_buf = (unsigned long)data_buf;
            if (ioctl(fd, SPI_IOC_MESSAGE(1), xfer) < 0)
                perror("SPI_IOC_MESSAGE");
            color_data_changed = false;
            color_data_ready.wait(lock, [] { return color_data_changed; });
        }
    }
    else {
        xfer[0].len = data_len;
        xfer[0].tx_buf = (unsigned long)data_buf;
        if (ioctl(fd, SPI_IOC_MESSAGE(1), xfer) < 0)
            perror("SPI_IOC_MESSAGE");
    }
    
    close(fd);
}
uint8_t* spi_recive(char* DEV_PATH, int spi_speed, uint8_t data_buf[], int data_len, int read_len)
{
    int fd;
    int ret;
    unsigned int mode, speed;
    struct spi_ioc_transfer xfer[2] = {0};
    uint8_t rec_buf[read_len];
    // open device node
    uint8_t* ret_buf = (uint8_t*)malloc(read_len);
    
    fd = open(DEV_PATH, O_RDWR);
    if (fd < 0) {
        printf("ERROR open %s ret=%d\n", DEV_PATH, fd);
    }

    // set spi mode
    mode = SPI_MODE_0;
    if (ioctl(fd, SPI_IOC_WR_MODE32, &mode) < 0) {
        printf("ERROR ioctl() set mode\n");
    }
    if (ioctl(fd, SPI_IOC_RD_MODE32, &ret) < 0) {
        printf("ERROR ioctl() get mode\n");
    } else
        printf("mode set to %d\n", (unsigned int)ret);

    // set spi speed
    speed = spi_speed;
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        printf("ERROR ioctl() set speed\n");
    }
    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &ret) < 0) {
        printf("ERROR ioctl() get speed\n");
    } else
        printf("speed set to %d\n", ret);
    
    xfer[0].len = data_len;
    xfer[0].tx_buf = (unsigned long)data_buf;
    xfer[1].len = read_len;
    xfer[1].rx_buf = (unsigned long)rec_buf;

    if (ioctl(fd, SPI_IOC_MESSAGE(2), xfer) < 0) {
        perror("SPI_IOC_MESSAGE");
        close(fd);
        return NULL;
    }
    
    memcpy(ret_buf, rec_buf, read_len);
    //free(rec_buf);
    close(fd);
    return ret_buf;
    
    
}