#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "function.h"
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
/*--------------PWM參數設置--pwm_init------------------
char* export_path -> PWM export路徑
char* period_path -> PWM 頻率設置路徑
char* duty_cycle_path -> PWM duty_cycle設置路徑
string period -> 頻率設置
string duty_cycle -> duty_cycle設置
-----------------------------------------------------*/
void pwm_init(char* export_path,char* period_path,char* duty_cycle_path,string period,string duty_cycle)
{
    size_t speed_len = period.length();
    size_t duty_cycle_len = duty_cycle.length();
    int fd, ret = 0;
    // echo 1 > export
    fd = open(export_path, O_WRONLY, 0);
    if (fd < 0)
    {
        printf("open file error");
    }
    printf("open export file success\n");
    
    ret = write(fd, "0", 1);
    if (ret < 0)
    {
        printf("write export error\n");
    }
    ret = write(fd, "1", 1);
    if (ret < 0)
    {
        printf("write export error\n");
    }
    ret = write(fd, "2", 1);
    if (ret < 0)
    {
        printf("write export error\n");
    }
    ret = write(fd, "3", 1);
    if (ret < 0)
    {
        printf("write export error\n");
    }
    close(fd);
    fd = open(period_path, O_WRONLY, 0);
    if (fd < 0)
    {
        printf("open file error");
    }
    printf("open period file success\n");
    
    ret = write(fd, period.c_str(), speed_len);
    if (ret < 0)
    {
        printf("write period error\n");
    }
    close(fd);

    
    fd = open(duty_cycle_path, O_WRONLY, 0);
    if (fd < 0)
    {
        printf("open file error");
    }
    printf("open duty_cycle file success\n");
    
    ret = write(fd, duty_cycle.c_str(), duty_cycle_len);
    if (ret < 0)
    {
        printf("write duty_cycle error\n");
    }
    close(fd);
}
/*--------------PWM致能--pwm_toggle-------------------
char* enable_path -> PWM 致能路徑
string toggle -> "1"為enable, "0"為disable
-----------------------------------------------------*/
void pwm_toggle(char* enable_path,string toggle)
{
    int fd = open(enable_path, O_WRONLY, 0);
    if (fd < 0)
    {
        printf("open file error");
    }
    printf("open enable file success\n");
    int ret = write(fd, toggle.c_str(), 1);
    if (ret < 0)
    {
        printf("write enable error\n");
    }
    close(fd);
}

