#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <linux/spi/spidev.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;





void spi_transmit(const char* DEV_PATH, int spi_speed, void* data_buf, int data_len, int bits);
uint8_t* spi_recive(const char* DEV_PATH, int spi_speed, void* data_buf, int data_len, int read_len, int bits);

#endif // SPI_H
