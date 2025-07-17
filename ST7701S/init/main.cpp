#include "spi.h"

void lcd_ini(){
    const char* SPI_PATH = "/dev/spidev1.1";
    uint16_t data1[6] = {0x0FF,0x177,0x101,0x100,0x100,0x113};
    uint16_t data2[2] = {0x0EF,0x108};
    uint16_t data3[6] = {0x0FF,0x177,0x101,0x100,0x100,0x101};
    
    //掃描行數
    uint16_t data4[3] = {0x0C0,0x163,0x100};
    //掃描前後空白行數
    uint16_t data5[3] = {0x0C1,0x10A,0x10C};
    
    uint16_t data6[3] = {0x0C2,0x107,0x107};
    uint16_t data7[2] = {0x0CC,0x138};

    uint16_t data8[17] = {0x0B0,0x1CE,0x15E,0x164,0x113,0x155,0x108,0x110,0x10C,0x10A,0x12B,0x106,0x1D0,0x10F,0x175,0x1FB,0x195};
    uint16_t data9[17] = {0x0B1,0x182,0x190,0x159,0x108,0x1CD,0x105,0x109,0x105,0x107,0x125,0x101,0x1D2,0x111,0x169,0x176,0x129};
    uint16_t data10[6] = {0x0FF,0x177,0x101,0x100,0x100,0x111};
    uint16_t data11[2] = {0x0B0,0x15D};
    uint16_t data12[2] = {0x0B1,0x131};
    uint16_t data13[2] = {0x0B2,0x180};
    uint16_t data14[2] = {0x0B3,0x180};
    uint16_t data15[2] = {0x0B5,0x145};
    uint16_t data16[2] = {0x0B7,0x185};
    uint16_t data17[2] = {0x0B8,0x121};
    uint16_t data18[3] = {0x0B9,0x110,0x11F};
    uint16_t data19[2] = {0x0BB,0x103};
    uint16_t data20[2] = {0x0BC,0x13E};
    uint16_t data21[2] = {0x0C1,0x178};
    uint16_t data22[2] = {0x0C2,0x178};
    uint16_t data23[2] = {0x0D0,0x188};
    uint16_t data24[4] = {0x0E0,0x100,0x100,0x102};
    uint16_t data25[12] = {0x0E1,0x104,0x100,0x100,0x100,0x105,0x100,0x100,0x100,0x100,0x120,0x120};
    uint16_t data26[13] = {0x0E2,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100};
    uint16_t data27[5] = {0x0E3,0x100,0x100,0x133,0x100};
    uint16_t data28[3] = {0x0E4,0x122,0x100};
    uint16_t data29[17] = {0x0E5,0x104,0x134,0x19A,0x1A0,0x106,0x134,0x19A,0x1A0,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100};
    uint16_t data30[5] = {0x0E6,0x100,0x100,0x133,0x100};
    uint16_t data31[3] = {0x0E7,0x122,0x100};
    uint16_t data32[17] = {0x0E8,0x105,0x134,0x19A,0x1A0,0x107,0x134,0x19A,0x1A0,0x100,0x100,0x100,0x100,0x100,0x100,0x100,0x100};
    uint16_t data33[8] = {0x0EB,0x102,0x100,0x140,0x140,0x100,0x100,0x100};
    uint16_t data34[3] = {0x0EC,0x100,0x100};
    uint16_t data35[17] = {0x0ED,0x1FA,0x145,0x10B,0x1FF,0x1FF,0x1FF,0x1FF,0x1FF,0x1FF,0x1FF,0x1FF,0x1FF,0x1FF,0x1B0,0x154,0x1AF};
    uint16_t data36[7] = {0x0EF,0x108,0x108,0x108,0x145,0x13F,0x154};
    uint16_t data37[6] = {0x0FF,0x177,0x101,0x100,0x100,0x113};
    uint16_t data38[3] = {0x0E8,0x100,0x10E};
    uint16_t data39[6] = {0x0FF,0x177,0x101,0x100,0x100,0x100};
    uint16_t data40[1] = {0x011};
    //delay(120)
    uint16_t data41[6] = {0x0FF,0x177,0x101,0x100,0x100,0x113};
    uint16_t data42[3] = {0x0E8,0x100,0x10C};
    //delay(10)
    uint16_t data43[3] = {0x0E8,0x100,0x100};
    uint16_t data44[6] = {0x0FF,0x177,0x101,0x100,0x100,0x113};
    uint16_t data45[3] = {0x0E6,0x116,0x17C};
    uint16_t data46[6] = {0x0FF,0x177,0x101,0x100,0x100,0x100};
    uint16_t data47[2] = {0x03A,0x155};
    uint16_t data48[1] = {0x029};
    int speed = 1*1000*1000;
    
    spi_transmit(SPI_PATH, speed, data1, sizeof(data1), 9);
    spi_transmit(SPI_PATH, speed, data2, sizeof(data2), 9);
    spi_transmit(SPI_PATH, speed, data3, sizeof(data3), 9);
    spi_transmit(SPI_PATH, speed, data4, sizeof(data4), 9);
    spi_transmit(SPI_PATH, speed, data5, sizeof(data5), 9);
    spi_transmit(SPI_PATH, speed, data6, sizeof(data6), 9);
    spi_transmit(SPI_PATH, speed, data7, sizeof(data7), 9);
    spi_transmit(SPI_PATH, speed, data8, sizeof(data8), 9);
    spi_transmit(SPI_PATH, speed, data9, sizeof(data9), 9);
    spi_transmit(SPI_PATH, speed, data10, sizeof(data10), 9);

    spi_transmit(SPI_PATH, speed, data11, sizeof(data11), 9);
    spi_transmit(SPI_PATH, speed, data12, sizeof(data12), 9);
    spi_transmit(SPI_PATH, speed, data13, sizeof(data13), 9);
    spi_transmit(SPI_PATH, speed, data14, sizeof(data14), 9);
    spi_transmit(SPI_PATH, speed, data15, sizeof(data15), 9);
    spi_transmit(SPI_PATH, speed, data16, sizeof(data16), 9);
    spi_transmit(SPI_PATH, speed, data17, sizeof(data17), 9);
    spi_transmit(SPI_PATH, speed, data18, sizeof(data18), 9);
    spi_transmit(SPI_PATH, speed, data19, sizeof(data19), 9);
    spi_transmit(SPI_PATH, speed, data20, sizeof(data20), 9);

    spi_transmit(SPI_PATH, speed, data21, sizeof(data21), 9);
    spi_transmit(SPI_PATH, speed, data22, sizeof(data22), 9);
    spi_transmit(SPI_PATH, speed, data23, sizeof(data23), 9);
    spi_transmit(SPI_PATH, speed, data24, sizeof(data24), 9);
    spi_transmit(SPI_PATH, speed, data25, sizeof(data25), 9);
    spi_transmit(SPI_PATH, speed, data26, sizeof(data26), 9);
    spi_transmit(SPI_PATH, speed, data27, sizeof(data27), 9);
    spi_transmit(SPI_PATH, speed, data28, sizeof(data28), 9);
    spi_transmit(SPI_PATH, speed, data29, sizeof(data29), 9);
    spi_transmit(SPI_PATH, speed, data30, sizeof(data30), 9);

    spi_transmit(SPI_PATH, speed, data31, sizeof(data31), 9);
    spi_transmit(SPI_PATH, speed, data32, sizeof(data32), 9);
    spi_transmit(SPI_PATH, speed, data33, sizeof(data33), 9);
    spi_transmit(SPI_PATH, speed, data34, sizeof(data34), 9);
    spi_transmit(SPI_PATH, speed, data35, sizeof(data35), 9);
    spi_transmit(SPI_PATH, speed, data36, sizeof(data36), 9);
    spi_transmit(SPI_PATH, speed, data37, sizeof(data37), 9);
    spi_transmit(SPI_PATH, speed, data38, sizeof(data38), 9);
    spi_transmit(SPI_PATH, speed, data39, sizeof(data39), 9);
    spi_transmit(SPI_PATH, speed, data40, sizeof(data40), 9);
    usleep(120*1000);
    spi_transmit(SPI_PATH, speed, data41, sizeof(data41), 9);
    spi_transmit(SPI_PATH, speed, data42, sizeof(data42), 9);
    usleep(10*1000);
    spi_transmit(SPI_PATH, speed, data43, sizeof(data43), 9);
    spi_transmit(SPI_PATH, speed, data44, sizeof(data44), 9);
    spi_transmit(SPI_PATH, speed, data45, sizeof(data45), 9);
    spi_transmit(SPI_PATH, speed, data46, sizeof(data46), 9);
    spi_transmit(SPI_PATH, speed, data47, sizeof(data47), 9);
    spi_transmit(SPI_PATH, speed, data48, sizeof(data48), 9);
    

}


int main() {
    lcd_ini();
    return 0;
}