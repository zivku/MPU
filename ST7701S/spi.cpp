#include "spi.h"
#include <cstring>

/*--------------SPI參數設置--spi_transmit-------------
char* DEV_SPI -> SPI路徑
int spi_speed -> SPI頻率設置
uint8_t data_buf[] -> 傳輸資料
int data_len -> 傳輸資料長度
int bits -> 單次傳輸的bit數
int state -> 傳輸模式, 1為連續傳輸 ,0為單筆傳輸
-----------------------------------------------------*/
void spi_transmit(const char* DEV_PATH, int spi_speed, void* data_buf, int data_len, int bits)
{
    int fd;
    int ret;
    unsigned int mode, speed , bits_per_word;
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
    
    bits_per_word = bits;
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
        printf("ERROR ioctl() set bits per word\n");
        close(fd);
    }
    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &ret) < 0) {
        printf("ERROR ioctl() get bits per word\n");
        close(fd);
    }

    
    xfer[0].len = data_len;
    xfer[0].tx_buf = (unsigned long)data_buf;
    xfer[0].bits_per_word = bits_per_word;
    if (ioctl(fd, SPI_IOC_MESSAGE(1), xfer) < 0)
        perror("SPI_IOC_MESSAGE");
    
    
    close(fd);
}
uint8_t* spi_recive(const char* DEV_PATH, int spi_speed, void* data_buf, int data_len, int read_len, int bits)
{
    int fd;
    int ret;
    unsigned int mode, speed, bits_per_word;
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
    
    bits_per_word = bits;
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) < 0) {
        printf("ERROR ioctl() set bits per word\n");
        close(fd);
    }
    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &ret) < 0) {
        printf("ERROR ioctl() get bits per word\n");
        close(fd);
    }


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