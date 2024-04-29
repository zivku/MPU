#include <egt/ui>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include "config.h"
#define window_width config.window_width
#define window_hight config.window_hight
Config config;
using WindowType = egt::Window;
static std::shared_ptr<WindowType> create_window(const std::string &name);
void txt_ini();
void write_txt(const char *text);
char* read_txt();
void pid_txt();
static std::shared_ptr<WindowType> create_window(const std::string &name)
{
    egt::Size size(window_width, window_hight);
    auto win = std::make_shared<WindowType>(size);
    win->name(name);
    return win;
}
void pid_txt() {
    pid_t pid = getpid();

    FILE *file = fopen("pid.txt", "w");

    if (file == NULL) {
        perror("open pid error\n\r");
    }

    fprintf(file, "%d", pid);
    fclose(file);
}
void txt_ini() {
    FILE *file;
    char filename[] = "lcd.txt";
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("creat new file%s\n", filename);

        
        file = fopen(filename, "w");
        if (file == NULL) {
            fprintf(stderr, "creat file error %s\n", filename);
        }
        fclose(file);
    } else {
        printf("file : %s existx pass\n", filename);
        fclose(file);
    }
}
void write_txt(const char *text) {
    FILE *file;
    char filename[] = "lcd.txt";

    file = fopen(filename, "w");

    if (file != NULL) {
        fprintf(file, "%s", text);
        fclose(file);
    } else {
        printf("Unable to open file %s\n", filename);
    }
}
char* read_txt() {
    FILE *file;
    char filename[] = "lcd.txt";
    char line[100];
    char* result = NULL;
    file = fopen(filename, "r");

    if (file != NULL) {
        if (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = 0;
            
            if (strstr(line, "page") != NULL || strstr(line, "data") != NULL) {
                fclose(file);

                file = fopen(filename, "w");
                fclose(file);

                result = strdup(line);
            }
            else {
                fclose(file);
                return NULL;
            }
        } else {
            fclose(file);
            return NULL;
        }
    } else {
        printf("Unable to open file %s\n", filename);
        return NULL;
    }

    return result;
}
int main(int argc, char** argv) {
    pid_txt();
    char page[100];
    int power, seconds;
    int animation_counter = 0;
    load_config("config.ini");
    egt::Application app(argc, argv);
    //創建主視窗
    egt::TopWindow win0;
    win0.width(window_width);
    win0.height(window_hight);
    win0.show();
    WindowType *parent = &win0;
    //設置畫面刷新頻率
    egt::PeriodicTimer timer1(std::chrono::milliseconds(100));
    //創建待機畫面
    auto IDLE_win = create_window("IDLE");
    IDLE_win->background(egt::Image("file:image/7_IDLE.jpg"));
    auto l3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7_qr_btn.png"));
    l3->move_to_center(egt::PointF(250, 0));
    l3->align(egt::AlignFlag::center_vertical);
    auto b5 = std::make_shared<egt::ImageButton>(egt::Image("file:image/7_rfid_btn.png"));
    b5->move_to_center(egt::PointF(500, 0));
    b5->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent);
    b5->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent, egt::Palette::GroupId::active);
    b5->align(egt::AlignFlag::center_vertical);
    //創建開啟RFID按鈕
    b5->on_click([](egt::Event &)
    { write_txt("state1"); });
    // IDLE_win->add(l11);
    IDLE_win->add(l3);
    IDLE_win->add(b5);
    parent->add(IDLE_win);
    //創建讀卡畫面
    auto RFID_win = create_window("RFID");
    RFID_win->background(egt::Image("file:image/RFID.jpg"));
    parent->add(RFID_win);
    //創建等待插槍畫面
    auto PLUG_IN_win = create_window("PLUG_IN");
    PLUG_IN_win->background(egt::Image("file:image/PLUG_IN.jpg"));
    parent->add(PLUG_IN_win);
    //創建充電中畫面
    auto CHARGING_win = create_window("CHARGING");
    CHARGING_win->background(egt::Image("file:image/7_CHARGING.jpg"));
    auto l2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/circle1.png"));
    l2->move_to_center(egt::PointF(200, 0));
    l2->align(egt::AlignFlag::center_vertical);
    auto b4 = std::make_shared<egt::ImageButton>(egt::Image("file:image/charge_btn.png"));
    b4->move_to_center(egt::PointF(550, 0));
    b4->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent);
    b4->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent, egt::Palette::GroupId::active);
    b4->align(egt::AlignFlag::center_vertical);
    b4->on_click([](egt::Event &)
    { write_txt("state2"); });
    CHARGING_win->add(l2);
    CHARGING_win->add(b4);
    parent->add(CHARGING_win);
    //創建結算畫面
    auto SETLEMENT_win = create_window("SETTLEMENT");
    SETLEMENT_win->background(egt::Image("file:image/7_bg.jpg"));
    // auto l12 = std::make_shared<egt::ImageLabel>(egt::Image("file:SETTLEMENT_text.png"));
    auto b10 = std::make_shared<egt::ImageButton>(egt::Image("file:image/7_SETTLEMENT.png"));

    b10->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent);
    b10->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent, egt::Palette::GroupId::active);
    b10->align(egt::AlignFlag::center);
    b10->on_click([](egt::Event &)
    { 
        write_txt("state0");
        //zero(); 
    });
    auto l10 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7_set.png"));
    l10->align(egt::AlignFlag::center);
    int energy_x = 610;
    int energy_y = 200;
    int time_x = 630;
    int time_y = 225;
    ///////////////////////////////////////////////////
    //創建累計功率個位數0-9數字
    auto energy_0_num_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto energy_0_num_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto energy_0_num_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto energy_0_num_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto energy_0_num_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto energy_0_num_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto energy_0_num_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto energy_0_num_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto energy_0_num_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto energy_0_num_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));
    energy_0_num_0->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_1->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_2->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_3->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_4->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_5->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_6->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_7->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_8->move_to_center(egt::PointF(energy_x, energy_y));
    energy_0_num_9->move_to_center(egt::PointF(energy_x, energy_y));
    ///////////////////////////////
    ///////////////////////////////////////////////////
    //創建累計功率十位數0-9數字
    auto energy_10_num_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto energy_10_num_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto energy_10_num_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto energy_10_num_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto energy_10_num_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto energy_10_num_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto energy_10_num_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto energy_10_num_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto energy_10_num_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto energy_10_num_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));
    energy_10_num_0->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_1->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_2->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_3->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_4->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_5->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_6->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_7->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_8->move_to_center(egt::PointF(energy_x, energy_y + 10));
    energy_10_num_9->move_to_center(egt::PointF(energy_x, energy_y + 10));

    ///////////////////////////////
    ///////////////////////////////////////////////////
    //創建累計功率百位數0-9數字
    auto energy_100_num_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto energy_100_num_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto energy_100_num_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto energy_100_num_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto energy_100_num_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto energy_100_num_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto energy_100_num_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto energy_100_num_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto energy_100_num_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto energy_100_num_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));

    energy_100_num_0->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_1->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_2->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_3->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_4->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_5->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_6->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_7->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_8->move_to_center(egt::PointF(energy_x, energy_y + 20));
    energy_100_num_9->move_to_center(egt::PointF(energy_x, energy_y + 20));

    ///////////////////////////////
    ///////////////////////////////////////////////////
    //創建累計功率千位數0-9數字
    auto energy_1000_num_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto energy_1000_num_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto energy_1000_num_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto energy_1000_num_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto energy_1000_num_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto energy_1000_num_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto energy_1000_num_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto energy_1000_num_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto energy_1000_num_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto energy_1000_num_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));

    energy_1000_num_0->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_1->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_2->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_3->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_4->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_5->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_6->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_7->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_8->move_to_center(egt::PointF(energy_x, energy_y + 30));
    energy_1000_num_9->move_to_center(egt::PointF(energy_x, energy_y + 30));
    //創建小時十位數0-9數字
    auto time_hour2_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto time_hour2_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto time_hour2_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto time_hour2_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto time_hour2_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto time_hour2_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto time_hour2_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto time_hour2_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto time_hour2_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto time_hour2_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));
    time_hour2_0->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_1->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_2->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_3->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_4->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_5->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_6->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_7->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_8->move_to_center(egt::PointF(time_x, time_y + 40));
    time_hour2_9->move_to_center(egt::PointF(time_x, time_y + 40));
    ///////////////////////////////
    ///////////////////////////////////////////////////
    //創建小時個位數0-9數字
    auto time_hour1_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto time_hour1_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto time_hour1_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto time_hour1_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto time_hour1_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto time_hour1_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto time_hour1_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto time_hour1_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto time_hour1_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto time_hour1_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));
    time_hour1_0->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_1->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_2->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_3->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_4->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_5->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_6->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_7->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_8->move_to_center(egt::PointF(time_x, time_y + 30));
    time_hour1_9->move_to_center(egt::PointF(time_x, time_y + 30));

    ///////////////////////////////
    ///////////////////////////////////////////////////
    //創建分鐘十位數0-9數字
    auto time_min2_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto time_min2_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto time_min2_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto time_min2_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto time_min2_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto time_min2_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto time_min2_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto time_min2_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto time_min2_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto time_min2_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));

    time_min2_0->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_1->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_2->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_3->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_4->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_5->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_6->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_7->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_8->move_to_center(egt::PointF(time_x, time_y + 10));
    time_min2_9->move_to_center(egt::PointF(time_x, time_y + 10));

    ///////////////////////////////
    ///////////////////////////////////////////////////
    //創建分鐘個位數0-9數字
    auto time_min1_0 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/0.png"));
    auto time_min1_1 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/1.png"));
    auto time_min1_2 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/2.png"));
    auto time_min1_3 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/3.png"));
    auto time_min1_4 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/4.png"));
    auto time_min1_5 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/5.png"));
    auto time_min1_6 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/6.png"));
    auto time_min1_7 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/7.png"));
    auto time_min1_8 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/8.png"));
    auto time_min1_9 = std::make_shared<egt::ImageLabel>(egt::Image("file:image/9.png"));

    time_min1_0->move_to_center(egt::PointF(time_x, time_y));
    time_min1_1->move_to_center(egt::PointF(time_x, time_y));
    time_min1_2->move_to_center(egt::PointF(time_x, time_y));
    time_min1_3->move_to_center(egt::PointF(time_x, time_y));
    time_min1_4->move_to_center(egt::PointF(time_x, time_y));
    time_min1_5->move_to_center(egt::PointF(time_x, time_y));
    time_min1_6->move_to_center(egt::PointF(time_x, time_y));
    time_min1_7->move_to_center(egt::PointF(time_x, time_y));
    time_min1_8->move_to_center(egt::PointF(time_x, time_y));
    time_min1_9->move_to_center(egt::PointF(time_x, time_y));
    ///////////////////////////////
    SETLEMENT_win->add(l10);
    SETLEMENT_win->add(energy_0_num_0);
    SETLEMENT_win->add(energy_0_num_1);
    SETLEMENT_win->add(energy_0_num_2);
    SETLEMENT_win->add(energy_0_num_3);
    SETLEMENT_win->add(energy_0_num_4);
    SETLEMENT_win->add(energy_0_num_5);
    SETLEMENT_win->add(energy_0_num_6);
    SETLEMENT_win->add(energy_0_num_7);
    SETLEMENT_win->add(energy_0_num_8);
    SETLEMENT_win->add(energy_0_num_9);

    SETLEMENT_win->add(energy_10_num_0);
    SETLEMENT_win->add(energy_10_num_1);
    SETLEMENT_win->add(energy_10_num_2);
    SETLEMENT_win->add(energy_10_num_3);
    SETLEMENT_win->add(energy_10_num_4);
    SETLEMENT_win->add(energy_10_num_5);
    SETLEMENT_win->add(energy_10_num_6);
    SETLEMENT_win->add(energy_10_num_7);
    SETLEMENT_win->add(energy_10_num_8);
    SETLEMENT_win->add(energy_10_num_9);

    SETLEMENT_win->add(energy_100_num_0);
    SETLEMENT_win->add(energy_100_num_1);
    SETLEMENT_win->add(energy_100_num_2);
    SETLEMENT_win->add(energy_100_num_3);
    SETLEMENT_win->add(energy_100_num_4);
    SETLEMENT_win->add(energy_100_num_5);
    SETLEMENT_win->add(energy_100_num_6);
    SETLEMENT_win->add(energy_100_num_7);
    SETLEMENT_win->add(energy_100_num_8);
    SETLEMENT_win->add(energy_100_num_9);

    SETLEMENT_win->add(energy_1000_num_0);
    SETLEMENT_win->add(energy_1000_num_1);
    SETLEMENT_win->add(energy_1000_num_2);
    SETLEMENT_win->add(energy_1000_num_3);
    SETLEMENT_win->add(energy_1000_num_4);
    SETLEMENT_win->add(energy_1000_num_5);
    SETLEMENT_win->add(energy_1000_num_6);
    SETLEMENT_win->add(energy_1000_num_7);
    SETLEMENT_win->add(energy_1000_num_8);
    SETLEMENT_win->add(energy_1000_num_9);

    SETLEMENT_win->add(time_hour2_0);
    SETLEMENT_win->add(time_hour2_1);
    SETLEMENT_win->add(time_hour2_2);
    SETLEMENT_win->add(time_hour2_3);
    SETLEMENT_win->add(time_hour2_4);
    SETLEMENT_win->add(time_hour2_5);
    SETLEMENT_win->add(time_hour2_6);
    SETLEMENT_win->add(time_hour2_7);
    SETLEMENT_win->add(time_hour2_8);
    SETLEMENT_win->add(time_hour2_9);

    SETLEMENT_win->add(time_hour1_0);
    SETLEMENT_win->add(time_hour1_1);
    SETLEMENT_win->add(time_hour1_2);
    SETLEMENT_win->add(time_hour1_3);
    SETLEMENT_win->add(time_hour1_4);
    SETLEMENT_win->add(time_hour1_5);
    SETLEMENT_win->add(time_hour1_6);
    SETLEMENT_win->add(time_hour1_7);
    SETLEMENT_win->add(time_hour1_8);
    SETLEMENT_win->add(time_hour1_9);

    SETLEMENT_win->add(time_min2_0);
    SETLEMENT_win->add(time_min2_1);
    SETLEMENT_win->add(time_min2_2);
    SETLEMENT_win->add(time_min2_3);
    SETLEMENT_win->add(time_min2_4);
    SETLEMENT_win->add(time_min2_5);
    SETLEMENT_win->add(time_min2_6);
    SETLEMENT_win->add(time_min2_7);
    SETLEMENT_win->add(time_min2_8);
    SETLEMENT_win->add(time_min2_9);

    SETLEMENT_win->add(time_min1_0);
    SETLEMENT_win->add(time_min1_1);
    SETLEMENT_win->add(time_min1_2);
    SETLEMENT_win->add(time_min1_3);
    SETLEMENT_win->add(time_min1_4);
    SETLEMENT_win->add(time_min1_5);
    SETLEMENT_win->add(time_min1_6);
    SETLEMENT_win->add(time_min1_7);
    SETLEMENT_win->add(time_min1_8);
    SETLEMENT_win->add(time_min1_9);

    

    SETLEMENT_win->add(b10);
    parent->add(SETLEMENT_win);

    //創建告警視窗
    auto ALARM_win = create_window("alarm");
    ALARM_win->background(egt::Image("file:image/ALERT.jpg"));
    auto b6 = std::make_shared<egt::ImageButton>(egt::Image("file:image/SETTLEMENT.jpg"));
    b6->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent);
    b6->color(egt::Palette::ColorId::button_bg, egt::Palette::transparent, egt::Palette::GroupId::active);
    b6->move_to_center(egt::PointF(0, 0));
    b6->align(egt::AlignFlag::center);
    b6->on_click([](egt::Event &)
    { 
        write_txt("state2"); 
    });
    ALARM_win->add(b6);
    parent->add(ALARM_win);

    txt_ini();
    write_txt("state0");

    timer1.on_timeout([&](){
        
        char* temp_page = read_txt();
        if (temp_page != NULL) {
            if (strstr(temp_page, "page") != NULL) {
                if (sscanf(temp_page, "page%s", page) == 1) {
                    printf("win page: %s\n", page);
                } else {
                    printf("win page error\n");
                }
            } else if (strstr(temp_page, "data") != NULL) {
                if (sscanf(temp_page, "data %d %d", &power, &seconds) == 2) {
                    printf("win data: %d, %d\n", power, seconds);
                } else {
                    printf("win data error\n");
                }
            }
            free(temp_page);
        } else {
            
        }
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        int remainingSeconds = seconds % 60;
        
        int hour_2 = (hours / 10) % 10;
        int hour_1 = hours % 10;
        int min_2 = (minutes / 10) % 10;
        int min_1 = minutes % 10;
        
        int energy_0 = power % 10;
        int energy_10 = (power / 10) % 10;
        int energy_100 = (power / 100) % 10;
        int energy_1000 = (power / 1000) % 10;

        if (strcmp(page, "1") == 0) {
            IDLE_win->show();
            RFID_win->hide();
            PLUG_IN_win->hide();
            CHARGING_win->hide();
            SETLEMENT_win->hide();
            ALARM_win->hide();
        }
        else if (strcmp(page, "2") == 0) {
            IDLE_win->hide();
            RFID_win->show();
            PLUG_IN_win->hide();
            CHARGING_win->hide();
            SETLEMENT_win->hide();
            ALARM_win->hide();
        }
        else if (strcmp(page, "3") == 0) {
            IDLE_win->hide();
            RFID_win->hide();
            PLUG_IN_win->show();
            CHARGING_win->hide();
            SETLEMENT_win->hide();
            ALARM_win->hide();
        }
        else if (strcmp(page, "4") == 0) {
            animation_counter = animation_counter+1;
            
            if (animation_counter == 0){
                l2-> image(egt::Image("file:image/circle1.png"));
            }
            else if (animation_counter == 1){
                l2-> image(egt::Image("file:image/circle2.png"));
            }
            else if (animation_counter == 2){
                l2-> image(egt::Image("file:image/circle3.png")); 
            }
            else if (animation_counter == 3){
                l2-> image(egt::Image("file:image/circle4.png")); 
            }
            else if (animation_counter == 4){
                l2-> image(egt::Image("file:image/circle5.png")); 
            }
            else if (animation_counter == 5){
                l2-> image(egt::Image("file:image/circle6.png")); 
            }
            else if (animation_counter == 6){
                l2-> image(egt::Image("file:image/circle7.png")); 
            }
            else if (animation_counter == 7){
                l2-> image(egt::Image("file:image/circle8.png")); 
            }
            else if (animation_counter == 8){
                l2-> image(egt::Image("file:image/circle9.png")); 
            }
            else if (animation_counter == 9){
                l2-> image(egt::Image("file:image/circle10.png")); 
            }
            else if (animation_counter == 10){
                l2-> image(egt::Image("file:image/circle11.png")); 
            }
            else if (animation_counter == 11){
                l2-> image(egt::Image("file:image/circle12.png")); 
            }
            else if (animation_counter == 12){
                l2-> image(egt::Image("file:image/circle13.png")); 
            }
            else if (animation_counter == 13){
                l2-> image(egt::Image("file:image/circle14.png")); 
            }
            else if (animation_counter == 14){
                l2-> image(egt::Image("file:image/circle15.png")); 
            }
            else if (animation_counter == 15){
                l2-> image(egt::Image("file:image/circle16.png")); 
            }
            else if (animation_counter == 16){
                l2-> image(egt::Image("file:image/circle17.png")); 
            }
            else if (animation_counter == 17){
                l2-> image(egt::Image("file:image/circle18.png")); 
            }
            else if (animation_counter == 18){
                l2-> image(egt::Image("file:image/circle19.png")); 
            }
            else if (animation_counter == 19){
                l2-> image(egt::Image("file:image/circle20.png")); 
            }
            else if (animation_counter == 20){
                l2-> image(egt::Image("file:image/circle21.png")); 
            }
            else if (animation_counter == 21){
                l2-> image(egt::Image("file:image/circle22.png")); 
            }
            else if (animation_counter == 22){
                l2-> image(egt::Image("file:image/circle23.png")); 
            }
            else if (animation_counter == 23){
                l2-> image(egt::Image("file:image/circle24.png")); 
            }
            else if (animation_counter == 24){
                l2-> image(egt::Image("file:image/circle25.png")); 
            }
            else if (animation_counter == 25){
                l2-> image(egt::Image("file:image/circle26.png")); 
            }
            else if (animation_counter == 26){
                l2-> image(egt::Image("file:image/circle27.png")); 
                animation_counter = 0;
            }
            IDLE_win->hide();
            RFID_win->hide();
            PLUG_IN_win->hide();
            CHARGING_win->show();
            SETLEMENT_win->hide();
            ALARM_win->hide();
        }
        else if (strcmp(page, "5") == 0) {
            ////////////////////////////////////////////////////
            if (energy_0 == 0){
            energy_0_num_0 ->show();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();
            }
            else if(energy_0 == 1){
            energy_0_num_0 ->hide();energy_0_num_1 ->show();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();
            }
            else if(energy_0 == 2){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->show();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();
            }
            else if(energy_0 == 3){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->show();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();
            }
            else if(energy_0 == 4){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->show();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();

            }
            else if(energy_0 == 5){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->show();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();

            }
            else if(energy_0 == 6){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->show();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->hide();

            }
            else if(energy_0 == 7){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->show();energy_0_num_8 ->hide();energy_0_num_9 ->hide();

            }
            else if(energy_0 == 8){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->show();energy_0_num_9 ->hide();

            }
            else if(energy_0 == 9){
            energy_0_num_0 ->hide();energy_0_num_1 ->hide();energy_0_num_2 ->hide();energy_0_num_3 ->hide();energy_0_num_4 ->hide();energy_0_num_5 ->hide();energy_0_num_6 ->hide();energy_0_num_7 ->hide();energy_0_num_8 ->hide();energy_0_num_9 ->show();
            }
            //////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
           if (energy_10 == 0){
            energy_10_num_0 ->show();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();
            }
            else if(energy_10 == 1){
            energy_10_num_0 ->hide();energy_10_num_1 ->show();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();
            }
            else if(energy_10 == 2){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->show();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();
            }
            else if(energy_10 == 3){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->show();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();
            }
            else if(energy_10 == 4){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->show();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();

            }
            else if(energy_10 == 5){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->show();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();

            }
            else if(energy_10 == 6){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->show();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->hide();

            }
            else if(energy_10 == 7){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->show();energy_10_num_8 ->hide();energy_10_num_9 ->hide();

            }
            else if(energy_10 == 8){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->show();energy_10_num_9 ->hide();

            }
            else if(energy_10 == 9){
            energy_10_num_0 ->hide();energy_10_num_1 ->hide();energy_10_num_2 ->hide();energy_10_num_3 ->hide();energy_10_num_4 ->hide();energy_10_num_5 ->hide();energy_10_num_6 ->hide();energy_10_num_7 ->hide();energy_10_num_8 ->hide();energy_10_num_9 ->show();
            }
            ////////////////////////////////////////
            ////////////////////////////////////////
            ///////////////////////////////////////
            if (energy_100 == 0){
            energy_100_num_0 ->show();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();
            }
            else if(energy_100 == 1){
            energy_100_num_0 ->hide();energy_100_num_1 ->show();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();
            }
            else if(energy_100 == 2){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->show();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();
            }
            else if(energy_100 == 3){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->show();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();
            }
            else if(energy_100 == 4){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->show();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();

            }
            else if(energy_100 == 5){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->show();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();

            }
            else if(energy_100 == 6){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->show();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->hide();

            }
            else if(energy_100 == 7){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->show();energy_100_num_8 ->hide();energy_100_num_9 ->hide();

            }
            else if(energy_100 == 8){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->show();energy_100_num_9 ->hide();

            }
            else if(energy_100 == 9){
            energy_100_num_0 ->hide();energy_100_num_1 ->hide();energy_100_num_2 ->hide();energy_100_num_3 ->hide();energy_100_num_4 ->hide();energy_100_num_5 ->hide();energy_100_num_6 ->hide();energy_100_num_7 ->hide();energy_100_num_8 ->hide();energy_100_num_9 ->show();
            }
            ///////////////////////////////////////////
            //////////////////////////////////////////
            /////////////////////////////////////////
            if (energy_1000 == 0){
            energy_1000_num_0 ->show();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();
            }
            else if(energy_1000 == 1){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->show();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();
            }
            else if(energy_1000 == 2){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->show();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();
            }
            else if(energy_1000 == 3){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->show();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();
            }
            else if(energy_1000 == 4){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->show();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();

            }
            else if(energy_1000 == 5){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->show();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();

            }
            else if(energy_1000 == 6){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->show();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();

            }
            else if(energy_1000 == 7){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->show();energy_1000_num_8 ->hide();energy_1000_num_9 ->hide();

            }
            else if(energy_1000 == 8){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->show();energy_1000_num_9 ->hide();

            }
            else if(energy_1000 == 9){
            energy_1000_num_0 ->hide();energy_1000_num_1 ->hide();energy_1000_num_2 ->hide();energy_1000_num_3 ->hide();energy_1000_num_4 ->hide();energy_1000_num_5 ->hide();energy_1000_num_6 ->hide();energy_1000_num_7 ->hide();energy_1000_num_8 ->hide();energy_1000_num_9 ->show();
            }
            if (hour_2 == 0){
            time_hour2_0 ->show();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();
            }
            else if(hour_2 == 1){
            time_hour2_0 ->hide();time_hour2_1 ->show();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();
            }
            else if(hour_2 == 2){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->show();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();
            }
            else if(hour_2 == 3){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->show();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();
            }
            else if(hour_2 == 4){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->show();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();

            }
            else if(hour_2 == 5){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->show();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();

            }
            else if(hour_2 == 6){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->show();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->hide();

            }
            else if(hour_2 == 7){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->show();time_hour2_8 ->hide();time_hour2_9 ->hide();

            }
            else if(hour_2 == 8){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->show();time_hour2_9 ->hide();

            }
            else if(hour_2 == 9){
            time_hour2_0 ->hide();time_hour2_1 ->hide();time_hour2_2 ->hide();time_hour2_3 ->hide();time_hour2_4 ->hide();time_hour2_5 ->hide();time_hour2_6 ->hide();time_hour2_7 ->hide();time_hour2_8 ->hide();time_hour2_9 ->show();
            }
            //////////////////////////////////////////////////////////////
            ////////////////////////////////////////////////////////////
            /////////////////////////////////////////////////////////////////////
            if (hour_1 == 0){
            time_hour1_0 ->show();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();
            }
            else if(hour_1 == 1){
            time_hour1_0 ->hide();time_hour1_1 ->show();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();
            }
            else if(hour_1 == 2){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->show();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();
            }
            else if(hour_1 == 3){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->show();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();
            }
            else if(hour_1 == 4){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->show();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();

            }
            else if(hour_1 == 5){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->show();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();

            }
            else if(hour_1 == 6){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->show();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->hide();

            }
            else if(hour_1 == 7){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->show();time_hour1_8 ->hide();time_hour1_9 ->hide();

            }
            else if(hour_1 == 8){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->show();time_hour1_9 ->hide();

            }
            else if(hour_1 == 9){
            time_hour1_0 ->hide();time_hour1_1 ->hide();time_hour1_2 ->hide();time_hour1_3 ->hide();time_hour1_4 ->hide();time_hour1_5 ->hide();time_hour1_6 ->hide();time_hour1_7 ->hide();time_hour1_8 ->hide();time_hour1_9 ->show();
            }
            ////////////////////////////////////////
            ////////////////////////////////////////
            ///////////////////////////////////////
            if (min_2 == 0){
            time_min2_0 ->show();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();
            }
            else if(min_2 == 1){
            time_min2_0 ->hide();time_min2_1 ->show();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();
            }
            else if(min_2 == 2){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->show();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();
            }
            else if(min_2 == 3){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->show();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();
            }
            else if(min_2 == 4){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->show();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();

            }
            else if(min_2 == 5){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->show();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();

            }
            else if(min_2 == 6){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->show();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->hide();

            }
            else if(min_2 == 7){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->show();time_min2_8 ->hide();time_min2_9 ->hide();

            }
            else if(min_2 == 8){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->show();time_min2_9 ->hide();

            }
            else if(min_2 == 9){
            time_min2_0 ->hide();time_min2_1 ->hide();time_min2_2 ->hide();time_min2_3 ->hide();time_min2_4 ->hide();time_min2_5 ->hide();time_min2_6 ->hide();time_min2_7 ->hide();time_min2_8 ->hide();time_min2_9 ->show();
            }
            ///////////////////////////////////////////
            //////////////////////////////////////////
            /////////////////////////////////////////
            if (min_1 == 0){
            time_min1_0 ->show();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();
            }
            else if(min_1 == 1){
            time_min1_0 ->hide();time_min1_1 ->show();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();
            }
            else if(min_1 == 2){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->show();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();
            }
            else if(min_1 == 3){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->show();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();
            }
            else if(min_1 == 4){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->show();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();

            }
            else if(min_1 == 5){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->show();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();

            }
            else if(min_1 == 6){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->show();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->hide();

            }
            else if(min_1 == 7){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->show();time_min1_8 ->hide();time_min1_9 ->hide();

            }
            else if(min_1 == 8){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->show();time_min1_9 ->hide();

            }
            else if(min_1 == 9){
            time_min1_0 ->hide();time_min1_1 ->hide();time_min1_2 ->hide();time_min1_3 ->hide();time_min1_4 ->hide();time_min1_5 ->hide();time_min1_6 ->hide();time_min1_7 ->hide();time_min1_8 ->hide();time_min1_9 ->show();
            }
            IDLE_win->hide();
            RFID_win->hide();
            PLUG_IN_win->hide();
            CHARGING_win->hide();
            SETLEMENT_win->show();
            ALARM_win->hide();
        }
        else if (strcmp(page, "6") == 0) {
            IDLE_win->hide();
            RFID_win->hide();
            PLUG_IN_win->hide();
            CHARGING_win->hide();
            SETLEMENT_win->hide();
            ALARM_win->show();
        }
    });
    timer1.start();
    win0.show();
    app.dump(std::cout);
    return app.run();
    return 0;
}