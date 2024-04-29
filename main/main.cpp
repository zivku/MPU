#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <thread>
#include <string>
#include <cmath>
// #include <egt/detail/string.h>
//#include <egt/ui>
#include <iostream>
#include <memory>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <fstream>
#include <linux/gpio.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <vector>
// #include <chrono>
#include "config.h"
#include "function.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <condition_variable>
#include <map>
///OCPP////
#include "stdio.h"
#include "ocpp_process.h"
#include "utilities/mytimer.h"
#include "starttransaction.h"
///////////

Config config;
#define GPIO_CHIP0 config.DEV_GPIO
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
#define RFID_PATH config.DEV_TTY
#define POWER_METER_PATH config.DEV_TTY2
#define LED_PATH config.DEV_SPI
#define SPI_METER_PATH config.SPI_METER
#define RELAY_PIN_1 config.GPIO_PIN_1 // PA25
#define RELAY_PIN_2 config.GPIO_PIN_2
#define SPEECH_PIN config.SPEECH_PIN 
#define window_width config.window_width
#define window_hight config.window_hight
#define LED_NUM  config.LED_NUM
#define J1772_CP_PATH config.PWM_ADC_PATH
#define PWM_SET_ADC_PATH config.PWM_SET_ADC_PATH
//-----------------PWM相關參數設置---------------------------------------------
#define PWM_export_PATH config.PWM_export_PATH         
#define PWM_period_PATH config.PWM_period_PATH          //設置頻率路徑
#define PWM_duty_cycle_PATH config.PWM_duty_cycle_PATH  //設置duty_cycle路徑
#define PWM_toggle_PATH config.PWM_enable_PATH          //致能PWM工作,需設置1才可訪問
////--------------------------------------------------------------
//----------------power_sensing pin define--------------------------
#define GND_ERR_PATH config.GND_ERR
#define L_ERR_PIN config.L_ERR
#define N_ERR_PIN config.N_ERR
#define GFCI_PIN config.GFCI
//------------------------------------------------------------------
#define speech_long 250
#define speech_short 10 
#define TIMEOUR_INIT config.TIMEOUT_INIT
#define TIMEOUT_6V config.TIMEOUT_6V
#define speech_path config.speech_path
#define speech_addr config.speech_addr 

#define OVER_VOLTAGE config.over_voltage
#define UNDER_VOLTAGE config.under_voltage
#define OVER_CURRENT config.over_current
#define OVER_TEMP config.over_temp
#define QLED_PATH config.qled_path
#define ICON_FLASHING_TIME config.flashing_time
#define TIMEOUT_STATE config.timeout_state
#define STOP_BTN config.stop_btn
#define DATE_TIME config.date_time
#define MAIN_DELAY config.main_delay
#define TEMP_ADC config.temp_adc
#define NO_VOLTAGE config.no_voltage
#define BREATHING_DELAY config.breathing_delay
#define STEADY_DELAY config.steady_delay
#define FLASHING_DELAY config.flashing_delay
#define TEST_DELAY config.test_delay
#define LIGHT_HIGHT config.light_hight
#define LIGHT_DOWN config.light_down
#define LERR_ADC config.lerr_adc
#define NERR_ADC config.nerr_adc
#define GNDERR_TEST_VOLTAGE config.gnderr_test_voltage
#define GFCI_TEST config.gfci_test
#define GFCI_TEST_HIGHT_DELAY config.gfci_test_hight_delay
#define GFCI_TEST_LOW_DELAY config.gfci_test_low_delay
#define LERR_DELTA config.lerr_delta
#define NERR_DELTA config.nerr_delta
#define L_COMPARISON config.l_comparison
#define N_COMPARISON config.n_comparison
uint8_t* color_data_buf;
int color_data_len;
int boot_status = 0;
std::mutex dataMutex;
std::string jsonData;
std::condition_variable dataReady;
std::map<std::string, Value> write_ocpp_json_data;
void speech_volume(int volume_num) {
    int i = 7;
    int volume_bit[8];
    while (volume_num > 0 && i >= 0) {
        volume_bit[i] = volume_num % 2;
        volume_num /= 2;
        i--;
    }
    while (i >= 0) {
        volume_bit[i] = 0;
        i--;
    }
    printf("volume :");
    for (int i = 0; i < 8; i++) {
        printf("%d", volume_bit[i]);
    }
    printf("\n\r");
    gpio_output(GPIO_CHIP0,SPEECH_PIN,0);
    usleep(3*1000);
    for (int i = 0; i < 8; i++) {
        if (volume_bit[i] == 0) {
            gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
            usleep(speech_short);
            gpio_output(GPIO_CHIP0,SPEECH_PIN,0);
            usleep(speech_long);
        }
        else {
            gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
            usleep(speech_long);
            gpio_output(GPIO_CHIP0,SPEECH_PIN,0);
            usleep(speech_short);
        }
    }
    gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
    usleep(3*1000);
}
void speech(int voice_num) {
    int i = 7;
    int voice_bit[8];
    while (voice_num > 0 && i >= 0) {
        voice_bit[i] = voice_num % 2;
        voice_num /= 2;
        i--;
    }
    while (i >= 0) {
        voice_bit[i] = 0;
        i--;
    }
    printf("speech : ");
    for (int i = 0; i < 8; i++) {
        printf("%d", voice_bit[i]);
    }
    printf("\n\r");
    gpio_output(GPIO_CHIP0,SPEECH_PIN,0);
    usleep(3*1000);
    for (int i = 0; i < 8; i++) {
        if (voice_bit[i] == 0) {
            gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
            usleep(speech_short);
            gpio_output(GPIO_CHIP0,SPEECH_PIN,0);
            usleep(speech_long);
        }
        else {
            gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
            usleep(speech_long);
            gpio_output(GPIO_CHIP0,SPEECH_PIN,0);
            usleep(speech_short);
        }
    }
    gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
    usleep(3*1000);
}
void ocpp_read_json(void) {
    const char* fifoPath = "/tmp/myfifo";
    mkfifo(fifoPath, 0666);
    int fd = open(fifoPath, O_RDONLY);

    if (fd < 0) {
        std::cerr << "Failed to open FIFO for reading." << std::endl;
    }

    while (1) {

        char buffer[4096];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    
        if (bytesRead < 0) {
            std::cerr << "Failed to read from FIFO." << std::endl;
            close(fd);
        }
        else {
            close(fd);

            buffer[bytesRead] = '\0';

            std::lock_guard<std::mutex> lock(dataMutex);
            jsonData = buffer;
            dataReady.notify_one();
        }
        fd = open(fifoPath, O_RDONLY);
        if (fd < 0) {
            std::cerr << "Failed to open FIFO for reading." << std::endl;
        }
        usleep(100*1000);
    }
}
void json_parsing() {
    while (true) {
        int chk_status = 0;
        std::unique_lock<std::mutex> lock(dataMutex);
        dataReady.wait(lock, [] { return !jsonData.empty(); });

        rapidjson::Document document;
        document.Parse(jsonData.c_str());

        if (document.HasParseError()) {
            std::cerr << "Failed to parse JSON data." << std::endl;
        }

        if (document.IsObject() && document.HasMember("title") && (document["title"] == write_ocpp_json_data["title"].GetString())) {
            std::map<std::string, Value> read_ocpp_json_Data;
            for (Value::ConstMemberIterator it = document.MemberBegin(); it != document.MemberEnd(); ++it) {
                const std::string key = it->name.GetString();
                if (it->value.IsString()) {
                    const std::string read_value = it->value.GetString();
                    const std::string write_value = write_ocpp_json_data[key].GetString();
                    Value v(read_value.c_str(), document.GetAllocator());
                    read_ocpp_json_Data[key] = v;
                    std::cout << "String read: " << key << " = " << read_value << std::endl;
                    std::cout << "String write: " << key << " = " << write_value << std::endl;
                    if (read_value == write_value) {
                        std::cout << "data key: " << key << " verification matches" << std::endl;
                    }
                    else {
                        std::cout << "data key: " << key << " verification does not match" << std::endl;
                        chk_status = 1;
                    }
                } 
                else if (it->value.IsInt()) {
                    int read_value = it->value.GetInt();
                    int write_value = write_ocpp_json_data[key].GetInt();
                    Value v(read_value);
                    read_ocpp_json_Data[key] = v;
                    std::cout << "Int read: " << key << " = " << read_value << std::endl;
                    std::cout << "Int write: " << key << " = " << write_value << std::endl;
                    if (read_value == write_value) {
                        std::cout << "data key: " << key << " verification matches" << std::endl;
                    }
                    else {
                        std::cout << "data key: " << key << " verification does not match" << std::endl;
                        chk_status = 1;
                    }
                } 
                else if (it->value.IsDouble()) {
                    double read_value = it->value.GetDouble();
                    double write_value = write_ocpp_json_data[key].GetDouble();
                    Value v(read_value);
                    read_ocpp_json_Data[key] = v;
                    std::cout << "Double read: " << key << " = " << read_value << std::endl;
                    std::cout << "Double write: " << key << " = " << write_value << std::endl;
                    if (read_value == write_value) {
                        std::cout << "data key: " << key << " verification matches" << std::endl;
                    }
                    else {
                        std::cout << "data key: " << key << " verification does not match" << std::endl;
                        chk_status = 1;
                    }
                } 
                else if (it->value.IsBool()) {
                    bool read_value = it->value.GetBool();
                    bool write_value = write_ocpp_json_data[key].GetBool();
                    Value v(read_value);
                    read_ocpp_json_Data[key] = v;
                    std::cout << "Bool read: " << key << " = " << (read_value ? "true" : "false") << std::endl;
                    std::cout << "Bool write: " << key << " = " << (write_value ? "true" : "false") << std::endl;
                    if (read_value == true) {
                        std::cout << key << " = " << (read_value ? "true" : "false") << std::endl;
                    }
                    else {
                        std::cout << key << " = " << (read_value ? "true" : "false") << std::endl;
                        chk_status = 1;
                    }
                }
            }
            if (chk_status == 0) {
                if (document.IsObject() && document["title"] == "boot") {
                    boot_status = 1;
                    printf("boot ok\n\r");
                }
                else if (document.IsObject() && document["title"] == "status") {
                    boot_status = 2;
                    printf("status ok\n\r");
                }
                else {
                    printf("wrong json request\n\r");
                }
            }
            else {
                boot_status = 0;
            }
            jsonData.clear();
        }
        else {
            if(document.IsObject() && document["title"] == "get") {
                if (document["project"] == "voltage")
                    printf("voltage info\n\r");
                    document["value"].SetInt(110);
                    document["request"].SetBool(true);
                
                ocpp_write_json(document);

            }
            else {
                printf("wrong json request\n\r");
            }
            
            
            jsonData.clear();
        }
    usleep(100*1000);
    }
}
rapidjson::Document mapToJSON(const std::map<std::string, rapidjson::Value>& mapData) {
    rapidjson::Document jsonDoc;
    jsonDoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

    for (const auto& pair : mapData) {
        rapidjson::Value key(pair.first.c_str(), allocator);
        rapidjson::Value value;

        if (pair.second.IsString()) {
            value.SetString(pair.second.GetString(), allocator);
        } else if (pair.second.IsInt()) {
            value.SetInt(pair.second.GetInt());
        } else if (pair.second.IsDouble()) {
            value.SetDouble(pair.second.GetDouble());
        } else if (pair.second.IsBool()) {
            value.SetBool(pair.second.GetBool());
        }

        jsonDoc.AddMember(key, value, allocator);
    }

    return jsonDoc;
}
int ocpp_write_json(const rapidjson::Document& jsonDoc) {
    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);
    jsonDoc.Accept(writer);
    const char* jsonString = buffer.GetString();
    const char* fifoPath = "/tmp/mypy";
    mkfifo(fifoPath, 0666);
    int fd = open(fifoPath, O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        std::cerr << "Failed to open FIFO for writing." << std::endl;
        return 1;
    }
    //while(fd != -1);
    write(fd, jsonString, strlen(jsonString));
    close(fd);
    return 0;
}
void boot(const char* model, const char* vendor, const char* serial, const char* firmware, const char* iccid, const char* imsi, const char* reason) {
    rapidjson::Value modelValue(rapidjson::StringRef(model));
    rapidjson::Value vendorValue(rapidjson::StringRef(vendor));
    rapidjson::Value serialValue(rapidjson::StringRef(serial));
    rapidjson::Value firmwareValue(rapidjson::StringRef(firmware));
    rapidjson::Value iccidValue(rapidjson::StringRef(iccid));
    rapidjson::Value imsiValue(rapidjson::StringRef(imsi));
    rapidjson::Value reasonValue(rapidjson::StringRef(reason)); 
    write_ocpp_json_data.clear();
    write_ocpp_json_data["title"] = "boot";
    write_ocpp_json_data["model"] =  modelValue; //char
    write_ocpp_json_data["vendor"] = vendorValue; //char
    write_ocpp_json_data["serial"] = serialValue; //char
    write_ocpp_json_data["firmware"] = firmwareValue; //char
    write_ocpp_json_data["iccid"] = iccidValue; //char
    write_ocpp_json_data["imsi"] = imsiValue; //char
    write_ocpp_json_data["reason"] = reasonValue; //char
    write_ocpp_json_data["request"] = false;

    rapidjson::Document jsonDoc = mapToJSON(write_ocpp_json_data);

    ocpp_write_json(jsonDoc);
    
}
void status(int evseID, int connectorID, const char* status) {
    rapidjson::Value evseIDValue(evseID);
    rapidjson::Value connectorIDValue(connectorID);
    rapidjson::Value statusValue(rapidjson::StringRef(status));
    write_ocpp_json_data.clear();
    write_ocpp_json_data["title"] = "status";
    write_ocpp_json_data["evseID"] = evseIDValue; //int
    write_ocpp_json_data["connectorID"] = connectorIDValue; //int
    write_ocpp_json_data["status"] = statusValue; //char ()
    write_ocpp_json_data["request"] = false;
    rapidjson::Document jsonDoc = mapToJSON(write_ocpp_json_data);

    ocpp_write_json(jsonDoc);
}
int j1772_cp_vehicle_status(int sample_rate) {
    if (sample_rate < 10) {
        sample_rate = 10;
    }
    else {
    }
    int low_count = (sample_rate * 9) / 10;
    int v12_len, v9_len, v6_len, v3_len, v0_len, low_len, fuzzy_len;
    int compare_index = 0;
    std::vector<int> ev_state_12v;
    std::vector<int> ev_state_9v;
    std::vector<int> ev_state_6v;
    std::vector<int> ev_state_3v;
    std::vector<int> ev_state_0v;
    std::vector<int> ev_state_low;
    std::vector<int> ev_state_fuzzy;
    std::vector<int> ev_state_compare;
    ev_state_12v.clear();
    ev_state_9v.clear();
    ev_state_6v.clear();
    ev_state_3v.clear();
    ev_state_0v.clear();
    ev_state_low.clear();
    ev_state_fuzzy.clear();
    for(int i = 0;i < sample_rate; i++) {
        int analog_single = adc(J1772_CP_PATH);
        float analog = static_cast<float>(analog_single);
        float digital_signal = analog / 16380;
        int ev_state = (24000 * digital_signal) - 11000;
        if (ev_state >= 11000) {
            ev_state_12v.push_back(ev_state);
        }
        else if ((10000 > ev_state) && (ev_state >= 8000)) {
            ev_state_9v.push_back(ev_state);
        }
        else if ((7000 >= ev_state) && (ev_state >= 5000)) {
            ev_state_6v.push_back(ev_state);
        }
        else if ((4000 >= ev_state) && (ev_state >= 2000)) {
            ev_state_3v.push_back(ev_state);
        }
        else if ((1000 >= ev_state) && (ev_state >= -1000)) {
            ev_state_0v.push_back(ev_state);
        }
        else if (ev_state < -1000) {
            ev_state_low.push_back(ev_state);
        }
        else {
            ev_state_fuzzy.push_back(ev_state);
        }
    }
    low_len = ev_state_low.size();
    v0_len = ev_state_0v.size();
    v3_len = ev_state_3v.size();
    v6_len = ev_state_6v.size();
    v9_len = ev_state_9v.size();
    v12_len = ev_state_12v.size();
    fuzzy_len = ev_state_fuzzy.size();
    printf("==========================\n");
    printf("-9V len : %d\n", low_len);
    printf("0V len : %d\n", v0_len);
    printf("3V len : %d\n", v3_len);
    printf("6V len : %d\n", v6_len);
    printf("9V len : %d\n", v9_len);
    printf("12V len : %d\n", v12_len);
    printf("fuzzy len : %d\n", fuzzy_len);
    printf("==========================\n");
    if (low_len < low_count) {
        ev_state_compare = {0, v0_len, v3_len, v6_len, v9_len, v12_len};
    }
    else {
        ev_state_compare = {low_len, v0_len, v3_len, v6_len, v9_len, v12_len};
    }
    
    auto max_state = std::max_element(ev_state_compare.begin(), ev_state_compare.end());
    compare_index = std::distance(ev_state_compare.begin(), max_state);
    int total = sample_rate - low_count;
    if (fuzzy_len >= total) {
        return 0;
    }
    return compare_index;
}
void relay_toggle(int state) {
    gpio_output(GPIO_CHIP0,RELAY_PIN_1,state);
    gpio_output(GPIO_CHIP0,RELAY_PIN_2,state);
}
float power_meter_voltage_read(void) {
    uint8_t voltage_read[] = {0xA5, 0x08, 0x41, 0x00, 0x06, 0x4E, 0x02, 0x44};
    size_t voltage_read_len = sizeof(voltage_read) / sizeof(voltage_read[0]);
    speed_t speed = B9600;
    int vtime = 1 * 10;
    int received_bytes = 0;
    int ret = 0;
    float ret_data = 0;
    uint8_t* recive = uart_transmit(POWER_METER_PATH, speed, vtime, voltage_read, voltage_read_len, received_bytes);
    if (recive[0] == 0x06) {
        printf("power meter voltage read success\n\r");
        ret = combineBytes(2, recive[3], recive[2]);
        ret_data = (float)ret;
        ret_data = ret_data / 10;
        
    }
    else {
        printf("power meter voltage read failed\n\r");
    }
    return ret_data;
}
float power_meter_current_read(void) {
    uint8_t current_read[] = {0xA5, 0x08, 0x41, 0x00, 0x0E, 0x4E, 0x04, 0x4E};
    size_t current_read_len = sizeof(current_read) / sizeof(current_read[0]);
    speed_t speed = B9600;
    int vtime = 1 * 10;
    int received_bytes = 0;
    int ret = 0;
    float ret_data = 0;
    uint8_t* recive = uart_transmit(POWER_METER_PATH, speed, vtime, current_read, current_read_len, received_bytes);
    if (recive[0] == 0x06) {
        printf("power meter current read success\n\r");
        ret = combineBytes(4, recive[5], recive[4], recive[3], recive[2]);
        ret_data = (float)ret;
        ret_data = ret_data / 10000 * 2;
    }
    else {
        printf("power meter current read failed\n\r");
    }
    return ret_data;
}
float power_meter_cumulative_power_read(void) {
    uint8_t cumulative_power_read[] = {0xA5, 0x08, 0x41, 0x00, 0x26, 0x4E, 0x08, 0x6A};
    size_t cumulative_power_read_len = sizeof(cumulative_power_read) / sizeof(cumulative_power_read[0]);
    speed_t speed = B9600;
    int vtime = 1 * 10;
    int received_bytes = 0;
    float ret_data = 0;
    int ret = 0;
    uint8_t* recive = uart_transmit(POWER_METER_PATH, speed, vtime, cumulative_power_read, cumulative_power_read_len, received_bytes);
    if (recive[0] == 0x06) {
        printf("power meter cumulative power read success\n\r");
        ret = combineBytes(8,recive[9], recive[8], recive[7], recive[6], recive[5], recive[4], recive[3], recive[2]);
        ret_data = (float)ret;
        ret_data = ret_data / 1000;
    }
    else {
        printf("power meter cumulative power read failed\n\r");
    }
    return ret_data;
}
int power_meter_cumulative_power_enable(void) {
    uint8_t cumulative_power_enable_cmd_1[] = {0xA5, 0x08, 0x41, 0x00, 0x94, 0x4E, 0x04, 0xD4};
    uint8_t cumulative_power_enable_cmd_2[] = {0xA5, 0x0C, 0x41, 0x00, 0x94, 0x4D, 0x04, 0x01, 0x80, 0x00, 0x03, 0x5B};
    size_t cumulative_power_enable_cmd_1_len = sizeof(cumulative_power_enable_cmd_1) / sizeof(cumulative_power_enable_cmd_1[0]);
    size_t cumulative_power_enable_cmd_2_len = sizeof(cumulative_power_enable_cmd_2) / sizeof(cumulative_power_enable_cmd_2[0]);
    speed_t speed = B9600;
    int vtime = 1 * 10;
    int received_cmd1_bytes = 0;
    int received_cmd2_bytes = 0;
    uint8_t* recive_cmd1 = uart_transmit(POWER_METER_PATH, speed, vtime, cumulative_power_enable_cmd_1, cumulative_power_enable_cmd_1_len, received_cmd1_bytes);
    uint8_t* recive_cmd2 = uart_transmit(POWER_METER_PATH, speed, vtime, cumulative_power_enable_cmd_2, cumulative_power_enable_cmd_2_len, received_cmd2_bytes);
    if ((recive_cmd1[0] == 0x06) & (recive_cmd2[0] == 0x06)) {
        printf("power meter cumulative power enable success\n\r");
        return 1;
        
    }
    else {
        printf("power meter cumulative power enable failed\n\r");
        return 0;
    }
}
int power_meter_cumulative_power_disable(void) {
    uint8_t cumulative_power_disable_cmd_1[] = {0xA5, 0x08, 0x41, 0x00, 0x94, 0x4E, 0x04, 0xD4};
    uint8_t cumulative_power_disable_cmd_2[] = {0xA5, 0x0C, 0x41, 0x00, 0x94, 0x4D, 0x04, 0x00, 0x80, 0x00, 0x03, 0x5A};
    size_t cumulative_power_disable_cmd_1_len = sizeof(cumulative_power_disable_cmd_1) / sizeof(cumulative_power_disable_cmd_1[0]);
    size_t cumulative_power_disable_cmd_2_len = sizeof(cumulative_power_disable_cmd_2) / sizeof(cumulative_power_disable_cmd_2[0]);
    speed_t speed = B9600;
    int vtime = 1 * 10;
    int received_cmd1_bytes = 0;
    int received_cmd2_bytes = 0;
    uint8_t* recive_cmd1 = uart_transmit(POWER_METER_PATH, speed, vtime, cumulative_power_disable_cmd_1, cumulative_power_disable_cmd_1_len, received_cmd1_bytes);
    uint8_t* recive_cmd2 = uart_transmit(POWER_METER_PATH, speed, vtime, cumulative_power_disable_cmd_2, cumulative_power_disable_cmd_2_len, received_cmd2_bytes);
    if ((recive_cmd1[0] == 0x06) & (recive_cmd2[0] == 0x06)) {
        printf("power meter cumulative power disable success\n\r");
        return 1;
        
    }
    else {
        printf("power meter cumulative power disable failed\n\r");
        return 0;
    }
}
int power_meter_cumulative_power_reset(void) {
    uint8_t cumulative_power_reset_cmd[] = {0xA5, 0x08, 0x41, 0x00, 0x26, 0x4E, 0x08, 0x6A};
    size_t cumulative_power_reset_cmd_len = sizeof(cumulative_power_reset_cmd) / sizeof(cumulative_power_reset_cmd[0]);
    speed_t speed = B9600;
    int vtime = 1 * 10;
    int received_bytes = 0;
    uint8_t* recive = uart_transmit(POWER_METER_PATH, speed, vtime, cumulative_power_reset_cmd, cumulative_power_reset_cmd_len, received_bytes);
    if (recive[0] == 0x06) {
        printf("power meter cumulative power reset success\n\r");
        return 1;
    }
    else {
        printf("power meter cumulative power reset failed\n\r");
        return 0;
    }
}
/*--------讀取RFID卡號-rfid_read-------
(CNT = one card) and (data xor pass) 
return card number
--------------------------------------*/
int rfid_read_card_no(uint8_t data[]) {
    uint8_t rfid_read_card_no_cmd[] = {0x00,0x00,0x05,0x91,0x01,0x01,0x00,0x11,0x85};
    size_t rfid_read_card_no_cmd_len = sizeof(rfid_read_card_no_cmd) / sizeof(rfid_read_card_no_cmd[0]);
    speed_t speed = B115200;
    int vtime = 5;
    int received_bytes = 0;
    uint8_t* recive = uart_transmit(RFID_PATH, speed, vtime, rfid_read_card_no_cmd, rfid_read_card_no_cmd_len, received_bytes);
    //uint8_t* recive = uart_read(RFID_PATH, speed, vtime, received_bytes);
    printf("rfid data bytes : %d\n\r",received_bytes);
    
    
    if (received_bytes > 10) {
        for (size_t i = 5; i < 15; i++) {
            data[i - 5] = recive[i];
            printf("%x\n\r", recive[i]);
        }
        
        return 1;
    }
    else {
        printf("card not detected\n\r");
        return -1;
    }
    // if ((recive[3] == 1) & (received_bytes != 0)) {
    // if (received_bytes != 0) {    
    //     for (size_t i = 5; i < 15; i++) {
    //         card_no += std::to_string(static_cast<int>(recive[i]));
    //         printf("%x\n\r",recive[i]);
    //     }
    //     return card_no;
    //     if (int ret = Xor(recive, received_bytes) == 1) {
    //         for (size_t i = 5; i < 15; i++) {
    //             card_no += std::to_string(static_cast<int>(recive[i]));
    //             printf("%d\n\r",recive[i]);
    //         }
    //         return card_no;
    //     }
    //     else {
    //         printf("RFID Xor Fail\n\r");
    //         return "0";
    //     }
    // }
    // else {
    //     printf("card number error\n\r");
    //     return "0";
    // }
}
/*--------蜂鳴器自檢-buzzer_detection--
3K clock 響 2秒
PWM 3k duty cycle 50%
--------------------------------------*/
void buzzer_detection(void) {
    string bz_speed = "333333";
    string bz_duty_cycle = "166667";
    pwm_init(PWM_export_PATH,PWM_period_PATH,PWM_duty_cycle_PATH,bz_speed,bz_duty_cycle);
    for (int i = 0; i < 3; i++) {
        pwm_toggle(PWM_toggle_PATH,"1");
        usleep(2*1000*1000);
        pwm_toggle(PWM_toggle_PATH,"0");
        usleep(1*1000*1000);
    }
}
/*----------------電源偵測---power_sensing--
	    Normal	Pass	Fail
N_ERR	Low	    Low	    High
L_ERR	Low	    Low 	High
GND_ERR	<0.5V	<0.5V	>0.5V
GFCI	Low	    Low	    High

-------------------------------------------*/
int power_sensing(void) {
    // for (int i = 0; i < 3; i++) {
    //     int lerr_ret = gpio_input(GPIO_CHIP0,L_ERR_PIN, 1);
    //     int nerr_ret = gpio_input(GPIO_CHIP0,N_ERR_PIN, 1);
    //     int gfci_ret = gpio_input(GPIO_CHIP0,GFCI_PIN, 1);
    //     int gnderr_ret = adc(GND_ERR_PATH);
    //     if ((lerr_ret == 1) & (nerr_ret == 1) & (gfci_ret == 0) & (gnderr_ret < 2500)) {
    //         printf("power sensing successful\n\r");
    //         return 1;
    //     }
    //     else {
    //         printf("power sensing Failed %d times\n\r",i); 
    //     }
    // }
    
    int lerr_ret = adc(LERR_ADC);
    int nerr_ret = adc(NERR_ADC);
    int gnderr_ret = adc(GND_ERR_PATH);
    int gfci_ret = gpio_input(GPIO_CHIP0,GFCI_PIN, 1);
    int ln_state = 0;
    if ((lerr_ret >= L_COMPARISON) & (nerr_ret <= N_COMPARISON)) {
        printf("L Phase Reversed with N Phase\n\r");
    }
    else if ((lerr_ret >= L_COMPARISON) & (nerr_ret > N_COMPARISON)) {
        printf("PE error\n\r");
    }
    else if ((lerr_ret < L_COMPARISON) & (nerr_ret < N_COMPARISON)) {
        printf("N Phase error\n\r");
    }
    else if ((lerr_ret < L_COMPARISON) & (nerr_ret > N_COMPARISON)) {
        ln_state = 1;
        printf(" L-N Phase Test Succeeded\n\r");
    }
    printf("LERR : %d , NERR : %d , GFCI : %d , GND : %d\n\r",lerr_ret,nerr_ret,gfci_ret,gnderr_ret);
    if ((ln_state == 1) & (gfci_ret == 0)) {
        printf("power sensing successful\n\r");
        return 1;
    }
    else {
        printf("power sensing Failed\n\r"); 
    }
    return 0;
    
}
int gfci_test(void) {
    gpio_output(GPIO_CHIP0, GFCI_TEST, 0);
    usleep(GFCI_TEST_HIGHT_DELAY);
    int gfci_ret = gpio_input(GPIO_CHIP0,GFCI_PIN, 1);
    if (gfci_ret == 1) {
        gpio_output(GPIO_CHIP0, GFCI_TEST, 1);
        usleep(GFCI_TEST_LOW_DELAY);
        int gfci_ret2 = gpio_input(GPIO_CHIP0,GFCI_PIN, 1);
        if (gfci_ret2 == 0) {
            printf("Successful Leakage Detection\n\r");
            return 1;
        }
        else {
            printf("Failure to Recover from Leakage Detection\n\r");
            return 0;
        }
    }
    else {
        printf("Failure in Leakage Detection\n\r");
        return 0;
    }
}
void led_ini(void){
    led_state = steady_black;
    color_data_len = set_led_data_length(LED_NUM);
    color_data_buf = set_color(color_data_len,224,0,0,0);
    int spi_speed = 2*1000*1000;
    std::thread SPI_thread(&spi_transmit, LED_PATH, spi_speed, color_data_buf, color_data_len, 1);
    SPI_thread.detach();
    std::thread led_thread(&led_mode, color_data_len, BREATHING_DELAY, STEADY_DELAY, FLASHING_DELAY, TEST_DELAY, LIGHT_HIGHT, LIGHT_DOWN);
    led_thread.detach();
}
void spi_meter_init() {
    uint8_t reset[4] = {0x00,0x00,0x78,0x9A};
    uint8_t sys0[2] = {0x80,0x01};
    uint8_t sys1[4] = {0x00,0x33,0x10,0x87};
    uint8_t config_start[4] = {0x00,0x30,0x87,0x65};
    uint8_t cal_start[4] = {0x00,0x40,0x87,0x65};
    uint8_t harm_start[4] = {0x00,0x50,0x87,0x65};
    uint8_t adj_start[4] = {0x00,0x60,0x87,0x65};
    uint8_t cs0[2] = {0x80,0x3B};
    uint8_t cs1[2] = {0x80,0x4D};
    uint8_t cs2[2] = {0x80,0x57};
    uint8_t cs3[2] = {0x80,0x6F};
    uint8_t cs0_write[4] = {0x00,0x3B,0x00,0x00};
    uint8_t cs1_write[4] = {0x00,0x4D,0x00,0x00};
    uint8_t cs2_write[4] = {0x00,0x57,0x00,0x00};
    uint8_t cs3_write[4] = {0x00,0x6F,0x00,0x00};
    uint8_t pga_gain[4] = {0x00,0x34,0x00,0xAA};
    uint8_t UgainA[4] = {0x00,0x61,0xCA,0xE4};
    uint8_t UgainB[4] = {0x00,0x65,0xCA,0xE4};
    uint8_t UgainC[4] = {0x00,0x69,0xCA,0xE4};
    uint8_t LgainA[4] = {0x00,0x62,0x77,0x99};
    uint8_t LgainB[4] = {0x00,0x66,0x77,0x99};
    uint8_t LgainC[4] = {0x00,0x6A,0x77,0x99};
    uint8_t LgainN[4] = {0x00,0x6D,0x77,0x99}; 
    int speed = 1*1000*1000;
    int write_len = 4;
    int len = 2;
    int read_len = 2;
    uint8_t* ret;
    spi_recive(SPI_METER_PATH, speed, reset, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, pga_gain, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, UgainA, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, UgainB, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, UgainC, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, LgainA, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, LgainB, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, LgainC, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, LgainN, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, sys1, write_len, 0);
    ret = spi_recive(SPI_METER_PATH, speed, cs0, len, read_len);
    cs0_write[2] = ret[0];
    cs0_write[3] = ret[1];
    ret = spi_recive(SPI_METER_PATH, speed, cs1, len, read_len);
    cs1_write[2] = ret[0];
    cs1_write[3] = ret[1];
    ret = spi_recive(SPI_METER_PATH, speed, cs2, len, read_len);
    cs2_write[2] = ret[0];
    cs2_write[3] = ret[1];
    ret = spi_recive(SPI_METER_PATH, speed, cs3, len, read_len);
    cs3_write[2] = ret[0];
    cs3_write[3] = ret[1];

    
    spi_recive(SPI_METER_PATH, speed, cs0_write, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, cs1_write, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, cs2_write, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, cs3_write, write_len, 0);

    spi_recive(SPI_METER_PATH, speed, config_start, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, cal_start, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, harm_start, write_len, 0);
    spi_recive(SPI_METER_PATH, speed, adj_start, write_len, 0);
    ret = spi_recive(SPI_METER_PATH, speed, sys0, len, read_len);
    printf("sys0 data[0] : %x\n\r", ret[0]);
    printf("sys0 data[1] : %x\n\r", ret[1]);

}
float spi_meter_read_energy() {
    int speed = 1*1000*1000;
    int len = 2;
    int read_len = 2;
    int energy_int = 0;
    float energy_float = 0;
    uint8_t* ret;
    uint8_t energy[2] = {0x80,0x84};
    uint8_t sys1[2] = {0x80,0x33};
    uint8_t high_bit[8];
    uint8_t low_bit[8];
    
    ret = spi_recive(SPI_METER_PATH, speed, sys1, len, read_len);
    printf("sys1 ret[0] : %02x\n\r", ret[0]);
    printf("sys1 ret[1] : %02x\n\r", ret[1]);
    byte_to_Bits(ret[0] ,high_bit);
    byte_to_Bits(ret[1] ,low_bit);
    ret = spi_recive(SPI_METER_PATH, speed, energy, len, read_len);
    for (int i = 0; i < read_len; i++) {
        // if (ret[i] == 0xFF) {
        //     ret[i] = 0x00;
        // }
        printf("rec data [%d]: %02x\n\r", i, ret[i]);
    }
    
    energy_int = combineBytes(2 , ret[0], ret[1]);
    energy_float = (float)energy_int;
    if (high_bit[1] == 0) {
        energy_float = (energy_float / 10) * 0.0003125;
    }
    else {
        energy_float = (energy_float / 100) * 0.0003125;
    }
    printf("ret float %.8f\n\r",energy_float);
    return energy_float;


}
int spi_meter_read_value() {
    int speed = 1*1000*1000;
    int len = 2;
    int read_len = 2;
    float scale = 0.201416015;
    uint8_t* ret = 0;
    uint8_t temp[2] = {0x80,0xFC};
    uint8_t UrmsA[2] = {0x80,0xD9};
    uint8_t UrmsB[2] = {0x80,0xDA};
    uint8_t UrmsC[2] = {0x80,0xDB};
    uint8_t LrmsN[2] = {0x80,0xDC};
    uint8_t LrmsA[2] = {0x80,0xDD};
    uint8_t LrmsB[2] = {0x80,0xDE};
    uint8_t LrmsC[2] = {0x80,0xDF};
    // ret = spi_recive(SPI_METER_PATH, speed, temp, len, read_len);
    // int t = combineBytes(2 , ret[0], ret[1]);
    // if (t > OVER_TEMP) {
    //     printf("meter temp error\n\r");
    //     return -1;
    // }
    int adc_temp = adc(TEMP_ADC);
    float main_temp = (float)adc_temp;
    float over_temp = (float)OVER_TEMP;
    main_temp = main_temp * scale;
    if(main_temp >= 500) {
        main_temp = (main_temp - 500) / 10;
    }
    else {
        main_temp = 0;
    }
    if (main_temp >= over_temp) {
        printf("temp error : %.3f over : %.3f\n\r",main_temp,over_temp);
        return -1;
    }
    printf("TEMP : %.3f\n\r", main_temp);
    ret = spi_recive(SPI_METER_PATH, speed, UrmsA, len, read_len);
    int av = (combineBytes(2 , ret[0], ret[1])) / 100;
    printf("A voltage : %d\n\r",av);
    if ((av > NO_VOLTAGE & (av < UNDER_VOLTAGE) | av > OVER_VOLTAGE)) {
        printf("meter A voltage error : %d under : %d over : %d\n\r", av, UNDER_VOLTAGE, OVER_VOLTAGE);
        return -1;
    } 
    ret = spi_recive(SPI_METER_PATH, speed, UrmsB, len, read_len);
    int bv = (combineBytes(2 , ret[0], ret[1])) / 100;
    printf("B voltage : %d\n\r",bv);
    if ((bv > NO_VOLTAGE & (bv < UNDER_VOLTAGE) | bv > OVER_VOLTAGE)) {
        printf("meter B voltage error : %d under : %d over : %d\n\r", bv, UNDER_VOLTAGE, OVER_VOLTAGE);
        return -1;
    } 
    ret = spi_recive(SPI_METER_PATH, speed, UrmsC, len, read_len);
    int cv = (combineBytes(2 , ret[0], ret[1])) / 100;
    printf("C voltage : %d\n\r",cv);
    if ((cv > NO_VOLTAGE & (cv < UNDER_VOLTAGE) | cv > OVER_VOLTAGE)) {
        printf("meter C voltage error : %d under : %d over : %d\n\r", cv, UNDER_VOLTAGE, OVER_VOLTAGE);
        return -1;
    } 
    ret = spi_recive(SPI_METER_PATH, speed, LrmsN, len, read_len);
    int nc = (combineBytes(2 , ret[0], ret[1])) / 1000;
    printf("N current : %d\n\r",nc);
    if (nc >= OVER_CURRENT) {
        printf("meter N current error : %d over : %d\n\r", nc, OVER_CURRENT);
        return -1;
    } 
    ret = spi_recive(SPI_METER_PATH, speed, LrmsA, len, read_len);
    int ac = (combineBytes(2 , ret[0], ret[1])) / 1000;
    printf("A current : %d\n\r",ac);
    if (ac >= OVER_CURRENT) {
        printf("meter A current error : %d over : %d\n\r", ac, OVER_CURRENT);
        return -1;
    } 
    ret = spi_recive(SPI_METER_PATH, speed, LrmsB, len, read_len);
    int bc = (combineBytes(2 , ret[0], ret[1])) / 1000;
    printf("B current : %d\n\r",bc);
    if (bc >= OVER_CURRENT) {
        printf("meter B current error : %d over : %d\n\r", bc, OVER_CURRENT);
        return -1;
    } 
    ret = spi_recive(SPI_METER_PATH, speed, LrmsC, len, read_len);
    int cc = (combineBytes(2 , ret[0], ret[1])) / 1000;
    printf("C current : %d\n\r",cc);
    if (cc >= OVER_CURRENT) {
        printf("meter C current error : %d over : %d\n\r", cc, OVER_CURRENT);
        return -1;
    } 
    return 1;

}
void qled_date(int state) {
    uint8_t cmd0_data2[8], cmd0_data3[8];
    uint8_t cmd1_data2[8], cmd1_data3[8];
    uint8_t cmd2_data2[8], cmd2_data3[8];
    uint8_t cmd3_data2[8], cmd3_data3[8];
    uint8_t cmd4_data2[8], cmd4_data3[8];
    uint8_t cmd5_data2[8], cmd5_data3[8]; 
    uint8_t cmd0_data1[8] = {0,0,0,0,0,0,0,0};
    uint8_t cmd1_data1[8] = {0,0,0,0,0,0,0,0};
    uint8_t cmd2_data1[8] = {0,0,0,1,0,0,0,0};
    uint8_t cmd3_data1[8] = {0,0,1,1,0,0,0,0};
    uint8_t cmd4_data1[8] = {0,0,0,0,0,0,0,0};
    uint8_t cmd5_data1[8] = {0,0,1,0,0,0,0,0};
    speed_t speed = B9600;
    int vtime = 1;
    size_t tx_len = 6;
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    // 取年、月、日
    int year = (localTime->tm_year + 1900) % 100;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;
    // 取時、分、秒
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;
    // 分別存儲時、分、秒的十位和個位數
    int hour1 = hour / 10;
    int hour2 = hour % 10;
    int minute1 = minute / 10;
    int minute2 = minute % 10;
    int second1 = second / 10;
    int second2 = second % 10;
    // 分別存儲年、月、日的十位和個位數
    int year1 = year / 10;
    int year2 = year % 10;
    int month1 = month / 10;
    int month2 = month % 10;
    int day1 = day / 10;
    int day2 = day % 10;
    value_to_bin(day2, cmd5_data2);
    value_to_bin(day1, cmd4_data2);
    value_to_bin(month2, cmd3_data2);
    value_to_bin(month1, cmd2_data2);
    value_to_bin(year2, cmd1_data2);
    value_to_bin(year1, cmd0_data2);
    value_to_bin(-1, cmd5_data3);
    value_to_bin(-1, cmd4_data3);
    value_to_bin(-1, cmd3_data3);
    value_to_bin(-1, cmd2_data3);
    value_to_bin(-1, cmd1_data3);
    value_to_bin(-1, cmd0_data3);
    if (state == 1) {
        cmd2_data1[3] = 1;
    }
    else {
        cmd2_data1[3] = 0;
    }
    uint8_t* cmd0 = set_led_bytes(0, cmd0_data1, cmd0_data2, cmd0_data3);
    uint8_t* cmd1 = set_led_bytes(1, cmd1_data1, cmd1_data2, cmd1_data3);
    uint8_t* cmd2 = set_led_bytes(2, cmd2_data1, cmd2_data2, cmd2_data3);
    uint8_t* cmd3 = set_led_bytes(3, cmd3_data1, cmd3_data2, cmd3_data3);
    uint8_t* cmd4 = set_led_bytes(4, cmd4_data1, cmd4_data2, cmd4_data3);
    uint8_t* cmd5 = set_led_bytes(5, cmd5_data1, cmd5_data2, cmd5_data3);
    uart_write(QLED_PATH, speed, vtime, cmd0, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd1, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd2, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd3, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd4, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd5, tx_len);
}
void qled_time(int state) {
    uint8_t cmd0_data2[8], cmd0_data3[8];
    uint8_t cmd1_data2[8], cmd1_data3[8];
    uint8_t cmd2_data2[8], cmd2_data3[8];
    uint8_t cmd3_data2[8], cmd3_data3[8];
    uint8_t cmd4_data2[8], cmd4_data3[8];
    uint8_t cmd5_data2[8], cmd5_data3[8]; 
    uint8_t cmd0_data1[8] = {0,0,0,1,0,0,0,0};
    uint8_t cmd1_data1[8] = {0,0,0,0,0,0,0,0};
    uint8_t cmd2_data1[8] = {0,0,1,1,0,0,0,0};
    uint8_t cmd3_data1[8] = {0,0,1,0,0,0,0,0};
    uint8_t cmd4_data1[8] = {0,0,1,0,0,0,0,0};
    uint8_t cmd5_data1[8] = {0,0,1,0,0,0,0,0};
    speed_t speed = B9600;
    int vtime = 1;
    size_t tx_len = 6;
    time_t currentTime;
    struct tm *localTime;
    time(&currentTime);
    localTime = localtime(&currentTime);
    // 取年、月、日
    int year = (localTime->tm_year + 1900) % 100;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;
    // 取時、分、秒
    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;
    int second = localTime->tm_sec;
    // 分別存儲時、分、秒的十位和個位數
    int hour1 = hour / 10;
    int hour2 = hour % 10;
    int minute1 = minute / 10;
    int minute2 = minute % 10;
    int second1 = second / 10;
    int second2 = second % 10;
    // 分別存儲年、月、日的十位和個位數
    int year1 = year / 10;
    int year2 = year % 10;
    int month1 = month / 10;
    int month2 = month % 10;
    int day1 = day / 10;
    int day2 = day % 10;
    value_to_bin(second2, cmd5_data2);
    value_to_bin(second1, cmd4_data2);
    value_to_bin(minute2, cmd3_data2);
    value_to_bin(minute1, cmd2_data2);
    value_to_bin(hour2, cmd1_data2);
    value_to_bin(hour1, cmd0_data2);
    value_to_bin(-1, cmd5_data3);
    value_to_bin(-1, cmd4_data3);
    value_to_bin(-1, cmd3_data3);
    value_to_bin(-1, cmd2_data3);
    value_to_bin(-1, cmd1_data3);
    value_to_bin(-1, cmd0_data3);
    if (state == 1) {
        cmd2_data1[3] = 1;
    }
    else {
        cmd2_data1[3] = 0;
    }
    uint8_t* cmd0 = set_led_bytes(0, cmd0_data1, cmd0_data2, cmd0_data3);
    uint8_t* cmd1 = set_led_bytes(1, cmd1_data1, cmd1_data2, cmd1_data3);
    uint8_t* cmd2 = set_led_bytes(2, cmd2_data1, cmd2_data2, cmd2_data3);
    uint8_t* cmd3 = set_led_bytes(3, cmd3_data1, cmd3_data2, cmd3_data3);
    uint8_t* cmd4 = set_led_bytes(4, cmd4_data1, cmd4_data2, cmd4_data3);
    uint8_t* cmd5 = set_led_bytes(5, cmd5_data1, cmd5_data2, cmd5_data3);
    uart_write(QLED_PATH, speed, vtime, cmd0, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd1, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd2, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd3, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd4, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd5, tx_len);
}
void qled_init(int mode) {
    uint8_t cmd0_data1[8], cmd0_data2[8], cmd0_data3[8];
    uint8_t cmd1_data1[8], cmd1_data2[8], cmd1_data3[8];
    uint8_t cmd2_data1[8], cmd2_data2[8], cmd2_data3[8];
    uint8_t cmd3_data1[8], cmd3_data2[8], cmd3_data3[8];
    uint8_t cmd4_data1[8], cmd4_data2[8], cmd4_data3[8];
    uint8_t cmd5_data1[8], cmd5_data2[8], cmd5_data3[8]; 
    speed_t speed = B9600;
    int vtime = 1;
    size_t tx_len = 6;
    value_to_bin(mode, cmd5_data1);
    value_to_bin(mode, cmd5_data2);
    value_to_bin(mode, cmd5_data3);
    value_to_bin(mode, cmd4_data1);
    value_to_bin(mode, cmd4_data2);
    value_to_bin(mode, cmd4_data3);
    value_to_bin(mode, cmd3_data1);
    value_to_bin(mode, cmd3_data2);
    value_to_bin(mode, cmd3_data3);
    value_to_bin(mode, cmd2_data1);
    value_to_bin(mode, cmd2_data2);
    value_to_bin(mode, cmd2_data3);
    value_to_bin(mode, cmd1_data1);
    value_to_bin(mode, cmd1_data2);
    value_to_bin(mode, cmd1_data3);
    value_to_bin(mode, cmd0_data1);
    value_to_bin(mode, cmd0_data2);
    value_to_bin(mode, cmd0_data3);
    uint8_t* cmd0 = set_led_bytes(0, cmd0_data1, cmd0_data2, cmd0_data3);
    uint8_t* cmd1 = set_led_bytes(1, cmd1_data1, cmd1_data2, cmd1_data3);
    uint8_t* cmd2 = set_led_bytes(2, cmd2_data1, cmd2_data2, cmd2_data3);
    uint8_t* cmd3 = set_led_bytes(3, cmd3_data1, cmd3_data2, cmd3_data3);
    uint8_t* cmd4 = set_led_bytes(4, cmd4_data1, cmd4_data2, cmd4_data3);
    uint8_t* cmd5 = set_led_bytes(5, cmd5_data1, cmd5_data2, cmd5_data3);
    uart_write(QLED_PATH, speed, vtime, cmd0, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd1, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd2, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd3, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd4, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd5, tx_len);
}
void qled_alarm(int mode) {
    uint8_t cmd0_data1[8], cmd0_data2[8], cmd0_data3[8];
    uint8_t cmd1_data1[8], cmd1_data2[8], cmd1_data3[8];
    uint8_t cmd2_data1[8], cmd2_data2[8], cmd2_data3[8];
    uint8_t cmd3_data1[8], cmd3_data2[8], cmd3_data3[8];
    uint8_t cmd4_data1[8], cmd4_data2[8], cmd4_data3[8];
    uint8_t cmd5_data1[8], cmd5_data2[8], cmd5_data3[8]; 
    speed_t speed = B9600;
    int vtime = 1;
    size_t tx_len = 6;
    value_to_bin(mode, cmd5_data1);
    value_to_bin(mode, cmd5_data2);
    value_to_bin(mode, cmd5_data3);
    value_to_bin(mode, cmd4_data1);
    value_to_bin(mode, cmd4_data2);
    value_to_bin(mode, cmd4_data3);
    value_to_bin(mode, cmd3_data1);
    value_to_bin(mode, cmd3_data2);
    value_to_bin(mode, cmd3_data3);
    value_to_bin(mode, cmd2_data1);
    value_to_bin(mode, cmd2_data2);
    value_to_bin(mode, cmd2_data3);
    value_to_bin(mode, cmd1_data1);
    value_to_bin(mode, cmd1_data2);
    value_to_bin(mode, cmd1_data3);
    value_to_bin(mode, cmd0_data1);
    value_to_bin(mode, cmd0_data2);
    value_to_bin(mode, cmd0_data3);
    cmd0_data1[2] = 1;
    uint8_t* cmd0 = set_led_bytes(0, cmd0_data1, cmd0_data2, cmd0_data3);
    uint8_t* cmd1 = set_led_bytes(1, cmd1_data1, cmd1_data2, cmd1_data3);
    uint8_t* cmd2 = set_led_bytes(2, cmd2_data1, cmd2_data2, cmd2_data3);
    uint8_t* cmd3 = set_led_bytes(3, cmd3_data1, cmd3_data2, cmd3_data3);
    uint8_t* cmd4 = set_led_bytes(4, cmd4_data1, cmd4_data2, cmd4_data3);
    uint8_t* cmd5 = set_led_bytes(5, cmd5_data1, cmd5_data2, cmd5_data3);
    uart_write(QLED_PATH, speed, vtime, cmd0, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd1, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd2, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd3, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd4, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd5, tx_len);
}
void charging_time(int seconds, int energy, bool* state, int checkout, int alarm) {
    uint8_t cmd0_data2[8], cmd0_data3[8];
    uint8_t cmd1_data2[8], cmd1_data3[8];
    uint8_t cmd2_data2[8], cmd2_data3[8];
    uint8_t cmd3_data2[8], cmd3_data3[8];
    uint8_t cmd4_data2[8], cmd4_data3[8];
    uint8_t cmd5_data2[8], cmd5_data3[8]; 
    uint8_t cmd0_data1[8] = {0,0,0,1,0,0,0,0};
    uint8_t cmd1_data1[8] = {0,0,1,0,0,0,0,0};
    uint8_t cmd2_data1[8] = {0,0,1,0,0,0,0,0};
    uint8_t cmd3_data1[8] = {0,0,1,0,0,0,0,0};
    uint8_t cmd4_data1[8] = {0,0,1,0,0,0,0,0};
    uint8_t cmd5_data1[8] = {0,0,1,0,0,0,0,0};
    speed_t speed = B9600;
    int vtime = 1;
    size_t tx_len = 6;
    int icon_state = 0;
    int hour = seconds / 3600;
    int minute = (seconds % 3600) / 60;
    int second = seconds % 60;
    int hour1 = hour / 10;
    int hour2 = hour % 10;
    int minute1 = minute / 10;
    int minute2 = minute % 10;
    int second1 = second / 10;
    int second2 = second % 10;

    int energy0, energy1, energy2, energy3, energy4, energy5;
    if (energy > 999999) {
        energy = 999999;
    }
    energy0 = energy / 100000;
    energy1 = (energy / 10000) % 10;
    energy2 = (energy / 1000) % 10;
    energy3 = (energy / 100) % 10;
    energy4 = (energy / 10) % 10;
    energy5 = energy % 10;
    if(count_digits(energy) <= 4) {
        energy0 = -1;
        energy1 = -1;
    }
    else if (count_digits(energy) == 5) {
        energy0 = -1;
    }
    icon_state = seconds / ICON_FLASHING_TIME;
    if (icon_state == 0) {
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd0_data1[0] = 1;
                *state = true;
            }
            else {
                cmd0_data1[0] = 0;
                *state = false;
            }
        }
            
    }
    else if(icon_state == 1) {
        cmd0_data1[0] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd1_data1[0] = 1;
                *state = true;
            }
            else {
                cmd1_data1[0] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 2) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd2_data1[0] = 1;
                *state = true;
            }
            else {
                cmd2_data1[0] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 3) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd3_data1[0] = 1;
                *state = true;
            }
            else {
                cmd3_data1[0] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 4) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd3_data1[0] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd4_data1[0] = 1;
                *state = true;
            }
            else {
                cmd4_data1[0] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 5) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd3_data1[0] = 1;
        cmd4_data1[0] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd5_data1[0] = 1;
                *state = true;
            }
            else {
                cmd5_data1[0] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 6) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd3_data1[0] = 1;
        cmd4_data1[0] = 1;
        cmd5_data1[0] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd5_data1[1] = 1;
                *state = true;
            }
            else {
                cmd5_data1[1] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 7) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd3_data1[0] = 1;
        cmd4_data1[0] = 1;
        cmd5_data1[0] = 1;
        cmd5_data1[1] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd4_data1[1] = 1;
                *state = true;
            }
            else {
                cmd4_data1[1] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 8) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd3_data1[0] = 1;
        cmd4_data1[0] = 1;
        cmd4_data1[1] = 1;
        cmd5_data1[0] = 1;
        cmd5_data1[1] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd3_data1[1] = 1;
                *state = true;
            }
            else {
                cmd3_data1[1] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 9) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd3_data1[0] = 1;
        cmd3_data1[1] = 1;
        cmd4_data1[0] = 1;
        cmd4_data1[1] = 1;
        cmd5_data1[0] = 1;
        cmd5_data1[1] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd2_data1[1] = 1;
                *state = true;
            }
            else {
                cmd2_data1[1] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 10) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd2_data1[0] = 1;
        cmd2_data1[1] = 1;
        cmd3_data1[0] = 1;
        cmd3_data1[1] = 1;
        cmd4_data1[0] = 1;
        cmd4_data1[1] = 1;
        cmd5_data1[0] = 1;
        cmd5_data1[1] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd1_data1[1] = 1;
                *state = true;
            }
            else {
                cmd1_data1[1] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state == 11) {
        cmd0_data1[0] = 1;
        cmd1_data1[0] = 1;
        cmd1_data1[1] = 1;
        cmd2_data1[0] = 1;
        cmd2_data1[1] = 1;
        cmd3_data1[0] = 1;
        cmd3_data1[1] = 1;
        cmd4_data1[0] = 1;
        cmd4_data1[1] = 1;
        cmd5_data1[0] = 1;
        cmd5_data1[1] = 1;
        if (checkout == 0 & alarm == 0) {
            if (*state == false){
                cmd0_data1[1] = 1;
                *state = true;
            }
            else {
                cmd0_data1[1] = 0;
                *state = false;
            }
        }
    }
    else if(icon_state >= 12) {
        cmd0_data1[0] = 1;
        cmd0_data1[1] = 1;
        cmd1_data1[0] = 1;
        cmd1_data1[1] = 1;
        cmd2_data1[0] = 1;
        cmd2_data1[1] = 1;
        cmd3_data1[0] = 1;
        cmd3_data1[1] = 1;
        cmd4_data1[0] = 1;
        cmd4_data1[1] = 1;
        cmd5_data1[0] = 1;
        cmd5_data1[1] = 1;
    }
    
    value_to_bin(second2, cmd5_data2);
    value_to_bin(energy5, cmd5_data3);
    value_to_bin(second1, cmd4_data2);
    value_to_bin(energy4, cmd4_data3);
    value_to_bin(minute2, cmd3_data2);
    value_to_bin(energy3, cmd3_data3);
    value_to_bin(minute1, cmd2_data2);
    value_to_bin(energy2, cmd2_data3);
    
    value_to_bin(hour2, cmd1_data2);
    value_to_bin(energy1, cmd1_data3);
    value_to_bin(hour1, cmd0_data2);
    value_to_bin(energy0, cmd0_data3);
    if (alarm == 1) {
        cmd0_data1[2] = 1;
    }
    else {
        cmd0_data1[2] = 0;
    }
    if (checkout == 1) {
        cmd2_data3[7] = 0;
        cmd1_data1[2] = 0;
        cmd1_data1[3] = 1;
        cmd2_data1[3] = 1;
    }
    else {
        cmd2_data3[7] = 1;
        cmd1_data1[2] = 1;
        cmd1_data1[3] = 0;
        cmd2_data1[3] = 0;
    }

    uint8_t* cmd0 = set_led_bytes(0, cmd0_data1, cmd0_data2, cmd0_data3);
    uint8_t* cmd1 = set_led_bytes(1, cmd1_data1, cmd1_data2, cmd1_data3);
    uint8_t* cmd2 = set_led_bytes(2, cmd2_data1, cmd2_data2, cmd2_data3);
    uint8_t* cmd3 = set_led_bytes(3, cmd3_data1, cmd3_data2, cmd3_data3);
    uint8_t* cmd4 = set_led_bytes(4, cmd4_data1, cmd4_data2, cmd4_data3);
    uint8_t* cmd5 = set_led_bytes(5, cmd5_data1, cmd5_data2, cmd5_data3);
    uart_write(QLED_PATH, speed, vtime, cmd0, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd1, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd2, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd3, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd4, tx_len);
    uart_write(QLED_PATH, speed, vtime, cmd5, tx_len);
}
void OCPP_CallBackFn(uint16_t frametype, uint16_t responcecode)
{
  printf("OCPP_CallBackFn ------->\n\r");
  printf("main: Frame Type: %d Frame code: %d\n\r", frametype, responcecode);

  switch (frametype)
  {
  case Authorize:
  {
    if (responcecode == AUTHORIZE_MISSING_PACKET_FIELD)
    {
      printf("Authorize packet issue\n\r");
    }

    AuthorizeResponseVal_t AuthorizeParams;
    GetAuthorizeParms(&AuthorizeParams);
    printf("Authorize parms for cache expiryDate: %s, parentIdTag: %s, status enum: %d, status: %s\n\r", AuthorizeParams.idTagInfo.expiryDate, AuthorizeParams.idTagInfo.parentIdTag, AuthorizeParams.statusENUM, AuthorizeParams.idTagInfo.status);
    // check if Authorization Cache implemented and add idTagInfo
    const char* key = "AuthorizationCacheEnabled";
    int cacheEnabled = chargerconfiguration_check_enabled_key(const_cast<char*>(key));
    if (cacheEnabled == 1)
    {
      printf("Authorization Cache Enabled\n");
    }
    else
    {
      printf("Authorization Cache Not Enabled\n");
    }
  }
  break;
  case BootNotification:
  {
    BootNotificationResponceVal_t BootNotificationParams;

    if (responcecode == BOOTNOTIFICATION_RESPONCE_ACCEPT_FAILED)
    {
      GetBootNotificationParms(&BootNotificationParams);
      printf("BootNotification Failed Retry interval will be: %u\n\r", BootNotificationParams.interval);
    }
  }
  break;
  case ChangeAvailability:
  {

    if (responcecode == CHANGEAVAILABILITY_RECEIVED)
    {
      uint8_t connectorid;
      char type[40];

      GetChangeAvailabilityParms(&connectorid, type);
      printf("OCPP_CallBackFn: I got: %d %s\n\r", connectorid, type);
      if (!strcmp(type, "Inoperative"))
      {
        SetChangeAvailabilityStatus(CHANGEAVAILABILITY_REJECTED);
      }
      else
        SetChangeAvailabilityStatus(CHANGEAVAILABILITY_ACCEPTED);
      printf("Done\n\r");
    }
  }
  break;
  case ClearCache:
  {
    // Clear Authorize Cache
    printf("Clear Cache \n\r");
    int result = 0;
    const char* key = "AuthorizationCacheEnabled";
    int cacheEnabled = chargerconfiguration_check_enabled_key(const_cast<char*>(key));
    if (cacheEnabled == 1)
    {
      result = 0; // TODO: Attempt to clear the cache and change result
      if (result == 1)
      {
        SetClearCacheStatus(CLEAR_CACHE_ACCEPTED);
      }
      else
      {
        SetClearCacheStatus(CLEAR_CACHE_REJECTED);
      }
    }
    else
    {
      SetClearCacheStatus(CLEAR_CACHE_REJECTED);
    }
  }
  break;
  case DataTransfer:
  {
    if (responcecode == DATA_TRANSFER_INITIATOR_CSMS)
    {
      RX_DataTransfer_Request_t DataTransferParms;
      GetDataTransferRequestParmsRX(&DataTransferParms);
      printf("DataTransfer Params Request: vendorId: %s messageId: %s hasData: %d data: %s\n\r", DataTransferParms.vendorId, DataTransferParms.messageId, DataTransferParms.hasData, DataTransferParms.data);
      // TODO: Check vendorID, messageID, Data etc.
      SetDataTransferResponseStatus(DATA_TRANSFER_ACCEPTED);
    }
    else if (responcecode == DATA_TRANSFER_INITIATOR_CHARGE_STATION)
    {
      RX_DataTransfer_Response_t DataTransferParms;
      GetDataTransferRequestParmsTX(&DataTransferParms);
      printf("DataTransfer Response Params: statusENUM: %d status: %s hasData: %d data: %s\n\r", DataTransferParms.statusENUM, DataTransferParms.status, DataTransferParms.hasData, DataTransferParms.data);
    }
    else if (responcecode == DATA_TRANSFER_INITIATOR_CHARGE_STATION_RESPONSE_ERROR)
    {
      printf("DataTransfer CSMS Response packet issue\n\r");
    }
  }
  break;
  case Heartbeat:
    break;
  case MeterValues:
    break;
  case Reset:
  {
    if (responcecode == RESET_HARD)
    {
      printf("Perform a Hard Reset\n\r");
      SetResetStatus(RESET_ACCEPTED);
    }
    else if (responcecode == RESET_SOFT)
    {
      printf("Perform a Soft Reset\n\r");
      SetResetStatus(RESET_ACCEPTED);
    }
  }
  break;
  case RemoteStartTransaction:
  {
    char idTag[20];
    int connectorId;
    GetRemoteStartidTagParms(idTag);
    GetRemoteStartconnectorIdParms(&connectorId);
    printf("Remote Start Transaction idTag: %s\n\r", idTag);
    printf("Remote Start Transaction connectorId: %d\n\r", connectorId);
    SetRemoteStartTransactionStatus(REMOTE_START_STOP_ACCEPTED);
  }
  break;
  case RemoteStopTransaction:
  {
    uint8_t transactionId;
    GetRemoteStopTransactionParms(&transactionId);
    printf("Remote Stop Transaction: %d", transactionId);
    // TODO: check if transactionId is ongoing
    SetRemoteStopTransactionStatus(REMOTE_START_STOP_ACCEPTED);
  }
  break;
  case StartTransaction:
  {
    char status[50];
    GetStartTransactionParmsStatus(status);
    printf("Start TX statis============%s=======",status);
  }
  break;
  case StopTransaction:
  {
    char status[50];
    GetStopTransactionParmsStatus(status);
    printf("Stop TX statis============%s=======",status);
  }
  break;
  case UnlockConnector:
  {
    uint8_t connectorid;
    GetUnlockConnectorParms(&connectorid);
    printf("UnlockConnector connectorId: %d \n\r", connectorid);
    // TODO: check if connectorId is known and whether connector was been unlocked or not
    SetUnlockConnectorStatus(UNLOCK_CONNECTOR_UNLOCKED);
  }
  break;
  case GetDiagnostics:
  {
    if (responcecode == GETDIAGNOSTICS_LOCATION_RECEIVED)
    {
      RX_GetDiagnostics_t params;
      GetGetDiagnosticsParms(&params);
      printf("GetDiagnostics Params location: %s startTime: %s stopTime: %s retryInterval: %d retries: %d\n", params.location, params.startTime, params.stopTime, params.retryInterval, params.retries);

      // Check for Diagnostics Info
      uint8_t hasInfo = 1;
      if (hasInfo == 0)
      {
        SetGetDiagnosticsHasInfo(0);
      }
      else
      {
        SetGetDiagnosticsHasInfo(1);
        const char* key = "GetDiagnostic_TestFileName.txt";
        SetGetDiagnosticsFileName(const_cast<char*>(key));
      }
    }
    else if (responcecode == GETDIAGNOSTICS_LOCATION_MISSING)
    {
      printf("GetDiagnostics Missing Location Parameter");
    }
  }
  break;
  case UpdateFirmware:
  {
    if (responcecode == UPDATEFIRMWARE_LOCATION_RECEIVED)
    {
      RX_UpdateFirmware_t params;
      GetUpdateFirmwareParms(&params);
      printf("UpdateFirmware Params location: %s retrieveDate: %s retryInterval: %d retries: %d\n", params.location, params.retrieveDate, params.retryInterval, params.retries);
    }
    else if (responcecode == UPDATEFIRMWARE_LOCATION_MISSING)
    {
      printf("UpdateFirmware Missing Location Parameter");
    }
  }
  break;
  case GetLocalListVersion:
  {
    int authListIsEmpty = 1;
    const char* key = "LocalAuthListEnabled";
    int authListEnabled = chargerconfiguration_check_enabled_key(const_cast<char*>(key));
    if (authListEnabled == 1)
    {
      authListIsEmpty = 1; // TODO: Check list version or 0 if empty
      if (authListIsEmpty == 1)
      {
        SetGetLocalListVersionListVersion(0);
      }
      else
      {
        int16_t versionListNum = 1;
        // TODO: add function to get versionList Number
        SetGetLocalListVersionListVersion(versionListNum);
      }
    }
    else
    {
      SetGetLocalListVersionListVersion(-1);
    }
  }
  break;
  case SendLocalList:
  {
    if (responcecode == SEND_LOCAL_LIST_MISSING_LIST_VERSION)
    {
      printf("SendLocalList request missing List Version Number\n");
    }
    else if (responcecode == SEND_LOCAL_LIST_MISSING_UPDATE_TYPE)
    {
      printf("SendLocalList request missing Missing Update Type\n");
    }
    else
    {
      RX_SendLocalList_t params;
      GetSendLocalListParams(&params);
      printf("SendLocalList Params listVersion: %d updateType: %d \n", params.listVersion, params.updateType);
      SetSendLocalListStatus(SEND_LOCAL_LIST_NOT_SUPPORTED);
    }
  }
  break;
  case TriggerMessage:
  {
    RX_TriggerMessage_t params;
    GetTriggerMessageParams(&params);
    printf("TriggerMessage Params connecterId: %d requestedMessage: %d %s\n", params.connecterId, params.requestedMessageType, params.requestedMessage);

    if (params.requestedMessageType == TRIGGER_MESSAGE_TYPE_INVALID)
    {
      SetTriggerMessageStatus(TRIGGER_MESSAGE_REJECTED);
    }
    else if (params.requestedMessageType == TRIGGER_MESSAGE_TYPE_NOT_IMPLEMENTED)

    {
      SetTriggerMessageStatus(TRIGGER_MESSAGE_NOT_IMPLEMENTED);
    }
    else
    {
      SetTriggerMessageStatus(TRIGGER_MESSAGE_ACCEPTED);
    }
  }
  break;
  case CancelReservation:
  {
    if (responcecode == CANCEL_RESERVATION_MISSING_RESERVATION_ID)
    {
      printf("CancelReservation request missing Reservation Id\n");
    }
    else
    {
      uint16_t params;
      GetCancelReservationParams(&params);
      printf("CancelReservation Params reservationId: %d\n", params);
      // TODO: Check Reservations for id
      //SetCancelReservationStatus(CANCEL_RESERVATION_ACCEPTED); // Set for testing
      SetCancelReservationStatus(static_cast<CLEAR_CACHE_STATUS_T>(CANCEL_RESERVATION_ACCEPTED));
      
    }
  }
  break;
  case ReserveNow:
  {
    if (responcecode == RESERVE_NOW_MISSING_CONNECTOR_ID)
    {
      printf("ReserveNow request missing Connector Id\n");
    }
    else if (responcecode == RESERVE_NOW_MISSING_EXPIRY_DATE)
    {
      printf("ReserveNow request missing Expirty Date\n");
    }
    else if (responcecode == RESERVE_NOW_MISSING_ID_TAG)
    {
      printf("ReserveNow request missing Id Tag\n");
    }
    else if (responcecode == RESERVE_NOW_MISSING_RESERVATION_ID)
    {
      printf("ReserveNow request missing Reservation Id\n");
    }
    else
    {
      RX_ReserveNow_t params;
      GetReserveNowParams(&params);
      printf("ReserveNow Params connectorId: %d expiryDate: %s idTag: %s parentIdTag: %s reservationId: %d\n", params.connectorId, params.expiryDate, params.idTag, params.parentIdTag, params.reservationId);
      // TODO: Attempt to Reserve
      SetReserveNowStatus(RESERVE_NOW_ACCEPTED); // Set for testing
    }
  }
  break;
  case StatusNotification:
  {

    if (responcecode == STATUSNOTIFICATION_RESPONCE_ACCEPT_FAILED)
    {
      printf("StatusNotification Failed \n\r");
    }
  }
  break;
  case DiagnosticsStatusNotification:
  {
    if (responcecode == DIAGNOSTICSTATUSNOTIFICATION_RESPONCE_ACCEPT_FAILED)
    {
      printf("DiagnosticStatusNotification Failed \n\r");
    }
  }
  break;
  case SetChargingProfile:
  {
    RX_SetChargingProfile_t params;
    GetSetChargingProfileParams(&params);

    SetSetChargingProfileStatus(SET_CHARGING_PROFILE_ACCEPTED);
  }
  break;
  case GetCompositeSchedule:
  {
    RX_GetCompositeSchedule_t params;

    if (responcecode == RX_GET_COMPOSITE_SCHEDULE_MISSING_CONNECTOR_ID)
    {
      SetGetCompositeScheduleStatus(GET_COMPOSITE_SCHEDULE_REJECTED);
    }
    else if (responcecode == RX_GET_COMPOSITE_SCHEDULE_MISSING_DURATION)
    {
      SetGetCompositeScheduleStatus(GET_COMPOSITE_SCHEDULE_REJECTED);
    }
    else
    {
      GetGetCompositeScheduleParams(&params);
      printf("GetCompositeSchedule Params connectorId: %d duration: %d chargingRateUnit: %s \n", params.connectorId, params.duration, params.chargingRateUnit);

      //SetGetCompositeScheduleStatus(SET_CHARGING_PROFILE_ACCEPTED);
      SetGetCompositeScheduleStatus(static_cast<GET_COMPOSITE_SCHEDULE_STATUS_T>(SET_CHARGING_PROFILE_ACCEPTED));
    }
  }
  break;
  case ClearChargingProfile:
  {
    RX_ClearChargingProfile_t params;
    GetClearChargingProfileParams(&params);

    //SetClearChargingProfileStatus(SET_CHARGING_PROFILE_ACCEPTED);
    SetClearChargingProfileStatus(static_cast<CLEAR_CHARGING_PROFILE_STATUS_T>(SET_CHARGING_PROFILE_ACCEPTED));
  }
  break;
  }
}
// void OCPP_CallBackFnMeterValue(SampledValue_t *sampledValue)
// {
//   switch (sampledValue->measurandENUM)
//   {
//   case METER_VALUES_ENERGY_ACTIVE_IMPORT_REGISTER:
//   {
//     double kwh = 55.64;
//     sprintf(sampledValue->value, "%0.2f", kwh);
//   }
//   break;
//   case METER_VALUES_VOLTAGE:
//   {
//     double voltage = 230;
//     sprintf(sampledValue->value, "%0.2f", voltage);
//   }
//   break;
//   }
// }

int kbhit()
{
  int byteswaiting;
  ioctl(0, FIONREAD, &byteswaiting);
  return byteswaiting > 0;
}


int main(int argc, char** argv) {
    load_config("config.ini");
    //can_send();
    int speed = 1*1000*1000;
    int len = 1;
    int read_len = 2;
    int energy_int = 0;
    float energy_float = 0;
    uint8_t* ret;
    uint8_t sys1[1] = {0xB2};
    /////OCPP/////
    char ch;
  

    // printf("This is OCPP Stack Test\n\r");

    // ocpp_init();
    // timer_initialize();
    // chargerconfiguration_database_init();
    // DataTransferInit();
    // MeterValueStructInit();
    //SendBootnotificationRequest();
    //SetAuthorize_idTag("23F532C35");
    //SendAuthorizeRequest();
    // SendDataTransferRequest();
    //SendMeterValuesRequest();
    // chargerconfiguration_database_close();
    while (1)
    {
        ch = getc(stdin);
        // printf("I received %c %02x\n", ch, ch);
        // const char* key = "23F532C35";
        if (ch == 0x0a)
        {
        printf("Received Enter\n\r");
        }
        else
        {
        switch (ch)
        {
            case 's':
            {
                printf("%x\n\r", speech_addr);
                uint8_t write_data[2] = {0x10,0xef};
                uint8_t play_data[4] = {0x7f,0x00,0x01,0x7f};
                for (int i = 0; i < 3; i++) {
                    i2c_read(speech_path, speech_addr, write_data, 2, 5);
                    i2c_write(speech_path, speech_addr, play_data, 4);
                    usleep(1000*1000);
                }
            }
            break;
            case 'l':
            {
                led_ini();
                led_state = breathing_blue;
            }
            break;
            case 'q':
            {
                qled_init(10);
                qled_alarm(-1);
            }
            break;
            case 'r':
            {
                uint8_t card_num[10];
                int ret = rfid_read_card_no(card_num);
                if (ret > 0) {
                    printf("card_num : ");
                    for (int i = 0; i < 10;i++){
                        printf("%d",card_num);
                    }
                    printf("\n\r");
                }
            }
            break;
            case 'c':
            {
                can_send();
            }
            break;

            
        }
        }
    }
    /////OCPP/////
    
    
    



    
    
    led_ini();
    spi_meter_init();
    qled_init(10);
    pwm_init(PWM_export_PATH, PWM_period_PATH, PWM_duty_cycle_PATH, "1000000", "500000");
    pwm_toggle(PWM_toggle_PATH, "0");
    relay_toggle(1);
    int state = 0;
    uint8_t card_num[10];
    uint8_t end_num[10];
    float energy = 0;
    int energy_display;
    time_t start_time, end_time, charging_seconds;
    time_t timeout_start, timeout_end, timeout;
    time_t date_start, date_end, datetime;
    bool date_state = false;
    bool flashing_state = false;
    printf("timeour : %d\n\r", TIMEOUT_STATE);
    date_start = time(NULL);
    int gfci_test_ret = gfci_test();
    int power_state = power_sensing();
    
    if (power_state != 1 | gfci_test_ret != 1) {
        qled_alarm(-1);
        pwm_toggle(PWM_toggle_PATH, "0");
        relay_toggle(1);
        led_state = steady_red;
        state = 5;
    }
    else {
        led_state = breathing_blue;
    }
    while (1) {
        printf("state : %d\n\r", state);
        // for (int i = 0; i < 10 ; i++) {
        //     printf("card num : %d\n\r", card_num[i]);
        //     printf("end num : %d\n\r", end_num[i]);
        // }
        int stop_btn = gpio_input(GPIO_CHIP0, STOP_BTN, 0);
        if (stop_btn == 0) {
            qled_alarm(-1);
            pwm_toggle(PWM_toggle_PATH, "0");
            relay_toggle(1);
            led_state = steady_red;
            state = 5;
        }
        if (state == 0) {
            date_end = time(NULL);
            datetime = date_end - date_start;
            
            if (datetime >= DATE_TIME) {
                date_start = time(NULL);
                date_state = !date_state;
            }
            if (date_state == false) {
                qled_date(1);
            }
            else {
                qled_time(1);
            }
            int ret = rfid_read_card_no(card_num);
            int power_state = power_sensing();
            if (ret > 0) {
                if (power_state == 1) {
                    int ret2 = gfci_test();
                    if (ret2 == 1) {
                        timeout_start = time(NULL);
                        date_start = time(NULL);
                        pwm_toggle(PWM_toggle_PATH, "1");
                        led_state = steady_blue;
                        state = 1;
                    }
                    else {
                        qled_alarm(-1);
                        pwm_toggle(PWM_toggle_PATH, "0");
                        relay_toggle(1);
                        led_state = steady_red;
                        state = 5;
                    }
                    
                }
                else {
                    qled_alarm(-1);
                    pwm_toggle(PWM_toggle_PATH, "0");
                    relay_toggle(1);
                    led_state = steady_red;
                    state = 5;
                }
            }
        }
        else if (state == 1) {
            date_end = time(NULL);
            datetime = date_end - date_start;
            if (datetime >= DATE_TIME) {
                date_start = time(NULL);
                date_state = !date_state;
            }
            if (date_state == false) {
                qled_date(0);
            }
            else {
                qled_time(0);
            }
            int ret = j1772_cp_vehicle_status(100);
            printf("index : %d \n\r",ret);
            if (ret == 2 | ret == 3) {
                relay_toggle(0);
                energy = 0;
                spi_meter_read_energy();
                start_time = time(NULL);
                led_state = breathing_green;
                state = 2;
                
            }
            timeout_end = time(NULL);
            timeout = timeout_end - timeout_start;
            if (timeout >= TIMEOUT_STATE) {
                led_state = breathing_blue;
                state = 0;


            }
            
        }
        else if (state == 2) {
            
            
            int ret = power_sensing();
            int ret2 = spi_meter_read_value();
            //int ret3 = gpio_input(GPIO_CHIP0, STOP_BTN, 0);
            int ret4 = j1772_cp_vehicle_status(100);
            int num = rfid_read_card_no(end_num);
            float data = spi_meter_read_energy();
            //bool ret5 = compareArrays(card_num, end_num, 10);
            int ret5 = memcmp(card_num, end_num, 10);
            // if (data >= 0.01) {
            //     data = 0;
            // }
            energy = energy + (data * 1000);
            energy_display = (int)energy;
            printf("energy int : %d\n\r",energy_display);
            end_time = time(NULL);
            charging_seconds = end_time - start_time;
            charging_time(charging_seconds, energy_display, &flashing_state, 0, 0);
            if (ret < 0 | ret2 < 0) {
                timeout_start = time(NULL);
                //charging_time(charging_seconds, energy_display, &flashing_state, 0, 1);
                qled_alarm(-1);
                pwm_toggle(PWM_toggle_PATH, "0");
                relay_toggle(1);
                led_state = steady_red;
                state = 5;
            }
            if ((ret5 == 0) | (ret4 != 2 & ret4 != 3)) {
                clearArray(end_num, 10);
                pwm_toggle(PWM_toggle_PATH, "0");
                relay_toggle(1);
                charging_time(charging_seconds, energy_display, &flashing_state, 1, 0);
                led_state = steady_green;

                state = 3;
            }

        }
        else if (state == 3) {
            int num = rfid_read_card_no(end_num);
            int ret = memcmp(card_num, end_num, 10);
            //bool ret = compareArrays(card_num, end_num, 10);
            if (ret == 0) {
                clearArray(end_num, 10);
                led_state = breathing_blue;
                state = 0;
            }
        }
        else if (state == 5) {
            int ret = power_sensing();
            int ret2 = spi_meter_read_value();
            int ret3 = gpio_input(GPIO_CHIP0, STOP_BTN, 0);
            int ret4 = gfci_test();
            if (ret > 0 & ret2 >= 0 & ret3 == 1 & ret4 == 1) {
                pwm_toggle(PWM_toggle_PATH, "0");
                relay_toggle(1);
                clearArray(end_num, 10);
                led_state = breathing_blue;
                state = 0;
            }
            
            // timeout_end = time(NULL);
            // timeout = timeout_end - timeout_start;
            // if (timeout >= TIMEOUT_STATE & gpio_input(GPIO_CHIP0, STOP_BTN, 0) == 1) {
            //     charging_time(charging_seconds, energy_display, &flashing_state, 1, 0);
            //     state = 3;
            // }
        }
        
        usleep(MAIN_DELAY);
    }
    
    
    
    
    

    


    // time_t last_time, charging_time;
    // char msg[100];
    // char result[100];
    // int timeout_count = 0;
    // int seconds = 0;
    // bool charging_state = false;
    // // buzzer_detection();
    // // power_sensing();
    // relay_toggle(1);
	// led_ini();
    // txt_ini();
    // power_meter_cumulative_power_disable();
    // power_meter_cumulative_power_reset();
    
    // pwm_init(PWM_export_PATH, PWM_period_PATH, PWM_duty_cycle_PATH, "1000000", "500000");
    // pwm_toggle(PWM_toggle_PATH, "0");
    // /*std::thread jsonThread(json_parsing);
    // std::thread readThread(ocpp_read_json);
    // readThread.detach();
    // jsonThread.detach();*/
    // //gpio_output(GPIO_CHIP0,SPEECH_PIN,1);
    
    // // while (boot_status == 0) {
    // //     boot("Wallbox XYZ", "anewone", "123", "456", "789", "101112", "PowerUp");
    // //     usleep(100*1000);
    // // }
    // // while (boot_status == 1) {
    // //     status(1234, 5678, "Faulted");
    // //     usleep(100*1000);
    // // }
    // int adc_data = adc(PWM_SET_ADC_PATH);
    // printf("adc data: %d\n\r", adc_data);
    
    
    // while (1) {
        
    //     /*int tt = gpio_input(GPIO_CHIP0,80);
    //     printf("gpio status : %d\n\r",tt);*/
    //     char* win_msg = read_txt();
    //     if (win_msg != NULL) {
    //         if (strstr(win_msg, "state") != NULL) {
    //             if (sscanf(win_msg, "state%s", msg) == 1) {
    //                 printf("main state: %s\n", msg);
    //             } 
    //             else {
    //                 printf("main state error\n");
    //             }
    //         }
    //         free(win_msg);
    //     } 
    //     else {
            
    //     }
    //     if (charging_state == true) {
    //         int ret = j1772_cp_vehicle_status(100);
    //         if (ret != 2 && ret != 3) {
    //             relay_toggle(1);
    //             pwm_toggle(PWM_toggle_PATH, "0");
    //             write_txt("page6");
    //             charging_state = false;
    //             led_state = flashing_red;
    //         }
    //         else {
    //             charging_time = time(NULL);
    //             seconds = charging_time - last_time;
    //             float power_wh = power_meter_cumulative_power_read();
    //             printf("float data : %f\n\r", power_wh);
    //             power_wh = power_wh * 1000;
    //             int power_int = (int)power_wh;
    //             printf("power data : %d %d\n\r", power_int, seconds);
    //             sprintf(result, "data %d %d", power_int, seconds);
                
    //         }
    //     }
    //     else {
    //         seconds = 0;
    //     }
    //     //int data = adc(J1772_CP_PATH);
    //     //printf("data : %d\n\r", data);
    //     if (strcmp(msg, "0") == 0) {
    //         write_txt("page1");
    //         memset(msg, 0, sizeof(msg));
    //         led_state = breathing_white;
    //     }
    //     else if (strcmp(msg, "1") == 0) {
    //         while(1) {
    //             int ret = j1772_cp_vehicle_status(100);
    //             if (ret == 5){
    //                 write_txt("page3");
    //                 pwm_toggle(PWM_toggle_PATH, "1");
    //                 led_state = breathing_green;
    //                 timeout_count = 0;
    //                 break;
    //             }
    //             else {
    //                 timeout_count = timeout_count + 1;
    //             }
    //             if (timeout_count >= TIMEOUR_INIT) {
    //                 write_txt("page1");
    //                 pwm_toggle(PWM_toggle_PATH, "0");
    //                 led_state = breathing_white;
    //                 timeout_count = 0;
    //                 break;
    //             }
    //             usleep(100*1000);
    //         }
    //         while (1) {
    //             int ret = j1772_cp_vehicle_status(100);
    //             if (ret == 2) {
    //                 write_txt("page4");
    //                 power_meter_cumulative_power_enable();
    //                 relay_toggle(0);
    //                 led_state = steady_green;
    //                 charging_state = true;
    //                 last_time = time(NULL);
    //                 timeout_count = 0;
    //                 break;
    //             }
    //             else if(ret == 3) {
    //                 write_txt("page4");
    //                 power_meter_cumulative_power_enable();
    //                 relay_toggle(0);
    //                 led_state = steady_green;
    //                 charging_state = true;
    //                 last_time = time(NULL);
    //                 timeout_count = 0;
    //                 break;
    //             }
    //             else {
    //                 timeout_count = timeout_count + 1;
    //             }
    //             if (timeout_count >= TIMEOUT_6V) {
    //                 write_txt("page1");
    //                 pwm_toggle(PWM_toggle_PATH, "0");
    //                 led_state = breathing_white;
    //                 timeout_count = 0;
    //                 break;
    //             }
    //             usleep(100*1000);
    //         }
    //         memset(msg, 0, sizeof(msg));
    //     }
    //     else if (strcmp(msg, "2") == 0) {
    //         write_txt(result);
    //         charging_state = false;
    //         pwm_toggle(PWM_toggle_PATH, "0");
    //         power_meter_cumulative_power_disable();
    //         relay_toggle(1);
    //         write_txt("page5");
    //         led_state = steady_blue;
    //         memset(msg, 0, sizeof(msg));
    //     }
    //     usleep(100*1000);
    // }
    
    
    
    return 0;
	
    

}
