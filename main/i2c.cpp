#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

/* EMC1414 I2C slave address */
//#define COMBINED_TRANSCTION

int i2c_write(char* DEV_I2C, int slave_addr, uint8_t write_data[], int data_len) {
    int fd;
    int ret;
    unsigned char buf[2];

    // open device node
    fd = open(DEV_I2C, O_RDWR);
    if (fd < 0) {
        printf("ERROR open %s ret=%d\n", DEV_I2C, fd);
        return -1;
    }

    if (ioctl(fd, I2C_SLAVE, slave_addr) < 0) {
        printf("ERROR ioctl() set slave address\n");
        return -1;
    }

    struct i2c_rdwr_ioctl_data data;
    struct i2c_msg messages[2];

    messages[0].addr  = slave_addr; //device address
    messages[0].flags = 0; //write
    messages[0].len   = data_len;
    messages[0].buf   = write_data; //data address

    data.msgs  = &messages[0];
    data.nmsgs = 1;
    if (ioctl(fd, I2C_RDWR, &data) < 0) {
        printf("ERROR ioctl() conversion rate\n");
        return -1;
    }

    // Read temperature
    // buf[0] = 0x10; // Internal Diode High Byte register address
    // buf[1] = 0;    // clear receive buffer
    // messages[0].addr  = slave_addr; //device address
    // messages[0].flags = 0; //write
    // messages[0].len   = 1;
    // messages[0].buf   = &buf[0]; //data address

    // messages[1].addr  = slave_addr; //device address
    // messages[1].flags = I2C_M_RD; //read
    // messages[1].len   = 1;
    // messages[1].buf   = &buf[1];

    // data.msgs  = messages;
    // data.nmsgs = 2;
    // while (1) {
    //     if (ioctl(fd, I2C_RDWR, &data) < 0) {
    //         printf("ERROR ioctl() read data\n");
    //         return -1;
    //     }

    //     printf("Temperature is %d\n", buf[1]);
    //     sleep(1);
    // }

    // close device node
    close(fd);

    return 0;
}

uint8_t* i2c_read(char* DEV_I2C, int slave_addr, uint8_t read_addr[], int data_len, int read_len) {
    int fd;
    int ret;

    // open device node
    fd = open(DEV_I2C, O_RDWR);
    if (fd < 0) {
        printf("ERROR open %s ret=%d\n", DEV_I2C, fd);
        return NULL;
    }

    if (ioctl(fd, I2C_SLAVE, slave_addr) < 0) {
        printf("ERROR ioctl() set slave address\n");
        return NULL;
    }

    struct i2c_rdwr_ioctl_data data;
    struct i2c_msg messages[2];
    //uint8_t rec_buf[read_len];
    uint8_t* rec_buf = new uint8_t[read_len];
    // Read temperature
    messages[0].addr  = slave_addr; //device address
    messages[0].flags = 0; //write
    messages[0].len   = data_len;
    messages[0].buf   = read_addr; //data address

    messages[1].addr  = slave_addr; //device address
    messages[1].flags = I2C_M_RD; //read
    messages[1].len   = read_len;
    messages[1].buf   = rec_buf;

    data.msgs  = messages;
    data.nmsgs = 2;
    
    if (ioctl(fd, I2C_RDWR, &data) < 0) {
        printf("ERROR ioctl() read data\n");
        return NULL;
    }
    else {
        for (int i = 0; i < read_len; i++) {
            printf("i2c read data[%d] : %02x\n\r", i, rec_buf[i]);
        }
        return rec_buf;
    }
    sleep(1);
    

    // close device node
    close(fd);

    return NULL;
}