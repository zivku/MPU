#include <cstdio>
#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <fcntl.h>   
#include <unistd.h>  
#include "function.h"
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
//float scale = 0.201416015;
int adc(char* adc_path)
{
    int fd, ret = 0;
    char buf[6] = "";
    fd = open(adc_path, O_RDONLY, 0);
    if (fd < 0)
    {
        printf("open file error\n");
    }
    ret = read(fd, buf, 5);
    if (ret < 0)
    {
        printf("write export error\n");        
    }
    int analog_single = std::atoi(buf);
    close(fd);
    
    return analog_single;            
}