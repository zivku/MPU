#include "function.h"
#include <iostream>
#include <unistd.h>
#include <mutex>
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
// int breathing_delay = 10*1000;
// int steady_delay = 100*1000;
// int flashing_delay = 500*1000;
// int test_delay = 100*1000;
STATE led_state;
int set_led_data_length(int led_num) {
    int ret = (8 + (led_num * 4));
    return ret;
}
uint8_t* set_color(int length, int brightness, int blue, int green, int red) {
    uint8_t* color_buf = new uint8_t[length];
    int flag = 0;
    
    for (int i = 0; i < 4; i++) {
        color_buf[i] = 0;
    }
    for (int i = 4; i < (length - 4); i++) {
        if (flag == 0) {
            color_buf[i] = brightness;
            flag++;
        }
        else if (flag == 1) {
            color_buf[i] = blue;
            flag++;
        }
        else if (flag == 2) {
            color_buf[i] = green;
            flag++;
        }
        else if (flag == 3) {
            color_buf[i] = red;
            flag++;
        }
        if (flag > 3) {
            flag = 0;
        }
    }
    for (int i = (length - 4); i < length; i++) {
        color_buf[i] = 255;
    }
    color_data_changed = true;
    color_data_ready.notify_all();
    return color_buf;
}
uint8_t* set_single(int length, int brightness, int blue, int green, int red, int light_num) {
    uint8_t* color_buf = new uint8_t[length];
    int flag = 0;
    
    for (int i = 0; i < 4; i++) {
        color_buf[i] = 0;
    }
    for (int i = 4; i <= (light_num * 4); i++) {
        if (flag == 0) {
            color_buf[i] = brightness;
            flag++;
        }
        else if (flag == 1) {
            color_buf[i] = blue;
            flag++;
        }
        else if (flag == 2) {
            color_buf[i] = green;
            flag++;
        }
        else if (flag == 3) {
            color_buf[i] = red;
            flag++;
        }
        if (flag > 3) {
            flag = 0;
        }
    }
    for (int i = ((light_num * 4) + 4); i < (length - 4); i++) {
        if (flag == 0) {
            color_buf[i] = 224;
            flag++;
        }
        else if (flag == 1) {
            color_buf[i] = 0;
            flag++;
        }
        else if (flag == 2) {
            color_buf[i] = 0;
            flag++;
        }
        else if (flag == 3) {
            color_buf[i] = 0;
            flag++;
        }
        if (flag > 3) {
            flag = 0;
        }
    }
    for (int i = (length - 4); i < length; i++) {
        color_buf[i] = 255;
    }
    color_data_changed = true;
    color_data_ready.notify_all();
    return color_buf;
}
void led_mode(int color_data_len, int breathing_delay, int steady_delay, int flashing_delay, int test_delay, int light_high, int light_down) {
    int led_num = (color_data_len - 8 ) / 4;
    // int light_high = 255;
    // int light_down = 0;
    while (1) {
        //std::unique_lock<std::mutex> lock(color_data_buf_mutex);
        switch (led_state) {
            
            case breathing_white:
                for (int i = light_high; i >= light_down; i--) {
                    //lock.lock();
                    if (led_state != breathing_white) {
                        break;
                    }
                    delete[] color_data_buf;
                    color_data_buf = set_color(color_data_len,255,i,i,i);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(breathing_delay);
                }
                for (int i = light_down; i <= light_high; i++) {
                    if (led_state != breathing_white) {
                        break;
                    }
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_color(color_data_len,255,i,i,i);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(breathing_delay);
                }
            break;
            case breathing_green:
                for (int i = light_high; i >= light_down; i--) {
                    if (led_state != breathing_green) {
                        break;
                    }
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_color(color_data_len,255,0,i,0);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(breathing_delay);
                }
                for (int i = light_down; i <= light_high; i++) {
                    if (led_state != breathing_green) {
                        break;
                    }
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_color(color_data_len,255,0,i,0);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(breathing_delay);
                }
            break;
            case breathing_blue:
                for (int i = light_high; i >= light_down; i--) {
                    if (led_state != breathing_blue) {
                        break;
                    }
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_color(color_data_len,255,i,0,0);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(breathing_delay);
                }
                for (int i = light_down; i <= light_high; i++) {
                    if (led_state != breathing_blue) {
                        break;
                    }
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_color(color_data_len,255,i,0,0);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(breathing_delay);
                }
            break;
            case steady_blue:
                //lock.lock();
                delete[] color_data_buf;
                color_data_buf = set_color(color_data_len,255,255,0,0);
                color_data_changed = true;
                //lock.unlock();
                color_data_ready.notify_all();
                usleep(steady_delay);
            break;
            case steady_green:
                //lock.lock();
                delete[] color_data_buf;
                color_data_buf = set_color(color_data_len,255,0,255,0);
                color_data_changed = true;
                //lock.unlock();
                color_data_ready.notify_all();
                usleep(steady_delay);
            break;
            case steady_red:
                //lock.lock();
                delete[] color_data_buf;
                color_data_buf = set_color(color_data_len,255,0,0,255);
                color_data_changed = true;
                //lock.unlock();
                color_data_ready.notify_all();
                usleep(steady_delay);
            break;
            case steady_black:
                //lock.lock();
                delete[] color_data_buf;
                color_data_buf = set_color(color_data_len,224,0,0,0);
                color_data_changed = true;
                //lock.unlock();
                color_data_ready.notify_all();
                usleep(steady_delay);
            break;
            case flashing_red:
                //lock.lock();
                if (led_state != flashing_red) {
                    break;
                }
                delete[] color_data_buf;
                color_data_buf = set_color(color_data_len,224,0,0,0);
                color_data_changed = true;
                //lock.unlock();
                color_data_ready.notify_all();
                usleep(flashing_delay);
                if (led_state != flashing_red) {
                    break;
                }
                //lock.lock();
                delete[] color_data_buf;
                color_data_buf = set_color(color_data_len,255,0,0,255);
                color_data_changed = true;
                //lock.unlock();
                color_data_ready.notify_all();
                usleep(flashing_delay);
            break;
            case test:
                for (int i = 1; i < led_num; i++) {
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_single(color_data_len,235,0,0,255,i);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(test_delay);
                }
                for (int i = led_num; i >= 1; i--) {
                    //lock.lock();
                    delete[] color_data_buf;
                    color_data_buf = set_single(color_data_len,235,0,0,255,i);
                    color_data_changed = true;
                    //lock.unlock();
                    color_data_ready.notify_all();
                    usleep(test_delay);
                }
            led_state = steady_black;
            break;

                
        }
    }
    
}