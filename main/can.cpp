/* 1. 报文发送程序 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>

void can_send()
{
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame[2] = {{0}};
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);//创建套接字
    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr); //指定 can0 设备
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));//将套接字与 can0 绑定
    //禁用过滤规则，本进程不接收报文，只负责发送
    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);
    //生成两个报文
    frame[0].can_id = 0x111;
    frame[0]. can_dlc = 1;
    frame[0].data[0] = 0x01;
    // frame[1].can_id = 0x888;
    // frame[1]. can_dlc = 1;
    // frame[1].data[0] = 0xee;
    //循环发送两个报文
    while(1)
    {
        nbytes = write(s, &frame[0], sizeof(frame[0])); //发送 frame[0]
        if(nbytes != sizeof(frame[0]))
        {
            printf("Send Error frame[0]\n!");
            break; //发送错误，退出
        }
        sleep(2);
        // nbytes = write(s, &frame[1], sizeof(frame[1])); //发送 frame[1]
        // if(nbytes != sizeof(frame[1]))
        // {
        //     printf("Send Error frame[1]\n!");
        //     break;
        // }
        sleep(2);
    }
    close(s);
}
void can_read()
{
    int s, nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter[1];
    s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    strcpy(ifr.ifr_name, "can0" );
    ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));
    rfilter[0].can_id = 0x11;
    rfilter[0].can_mask = CAN_SFF_MASK;

    setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    while(1)
    {
        nbytes = read(s, &frame, sizeof(frame));
    
        if(nbytes > 0)
        {
            printf("ID=0x%X DLC=%d data[0]=0x%X\n", frame.can_id,
                frame.can_dlc, frame.data[0]);
        }
    }
    close(s);
}
int can_test()
{
	int s; 
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	printf("CAN Sockets Demo\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, "can0" );
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	frame.can_id = 0x555;
	frame.can_dlc = 1;
    frame.data[0] = 0x12;
	//sprintf(frame.data, "Hello");

	if (write(s, &frame, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
		perror("Write");
		return 1;
	}

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}