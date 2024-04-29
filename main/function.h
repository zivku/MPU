#ifndef FUNCTION_H
#define FUNCTION_H
#include <iostream>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <termios.h>
#include <map>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
using namespace std;
using std::cout;
using std::endl;
using std::reverse;
using std::string;
enum STATE
{
    test = 0,
    breathing_white = 1,
    breathing_green = 2,
    steady_blue = 3,
    steady_green = 4,
    steady_black = 5,
    flashing_red = 6,
    breathing_blue = 7,
    steady_red = 8,
};
extern std::mutex color_data_buf_mutex; 
extern std::condition_variable color_data_ready; 
extern STATE led_state;
extern uint8_t* color_data_buf; 
extern bool color_data_changed;
void pwm_init(char* export_path,char* period_path,char* duty_cycle_path,string period,string duty_cycle);
void pwm_toggle(char* enable_path,string toggle);
void gpio_output(char* gpio_path,int gpio_pin,int value);
int gpio_input(char* gpio_path,int gpio_pin, int mode);
int adc(char* adc_path);

uint8_t* uart_transmit(char* DEV_PATH, speed_t baudRate, int vtime, uint8_t tx_data[], int tx_len, int&received_bytes);
uint8_t* uart_read(char* DEV_PATH, speed_t baudRate, int vtime, int&received_bytes);
int uart_write(char* DEV_PATH, speed_t baudRate, int vtime, uint8_t tx_data[], int tx_len);

void spi_transmit(char* DEV_PATH, int spi_speed, uint8_t data_buf[], int data_len, int state);
uint8_t* spi_recive(char* DEV_PATH, int spi_speed, uint8_t data_buf[], int data_len, int read_len);


uint8_t* set_color(int length, int brightness, int blue, int green, int red);
uint8_t* set_single(int length, int brightness, int blue, int green, int red, int light_num);
void led_ini(void);
void led_mode(int color_data_len, int breathing_delay, int steady_delay, int flashing_delay, int test_delay, int light_high, int light_down);
int set_led_data_length(int led_num);
void set_led_mode(int length, uint8_t color_buf[]);

void buzzer_detection(void);
int power_sensing(void);
int gfci_test(void);
int rfid_read_card_no(uint8_t data[]);

int Xor(uint8_t data[], size_t data_len);
int combineBytes(int bytes, ...);
void byte_to_Bits(uint8_t byte, uint8_t bits[8]);

float power_meter_voltage_read(void);
float power_meter_current_read(void);
float power_meter_cumulative_power_read(void);
int power_meter_cumulative_power_enable(void);
int power_meter_cumulative_power_disable(void);
int power_meter_cumulative_power_reset(void);

void relay_toggle(int state);
int j1772_cp_vehicle_status(int sample_rate);
void j1772_cp_pwm(string duty_cycle);

int ocpp_write_json(const rapidjson::Document& jsonDoc);
void json_parsing(void);
void ocpp_read_json(void);
void boot(const char* model, const char* vendor, const char* serial, const char* firmware, const char* iccid, const char* imsi, const char* reason);
void status(int evseID, int connectorID, const char* status);
rapidjson::Document mapToJSON(const std::map<std::string, rapidjson::Value>& mapData);

void txt_ini(void);
void write_txt(const char *text);
char* read_txt(void);

void speech(int voice_num);
void speech_volume(int volume_num);

int i2c_write(char* DEV_I2C, int slave_addr, uint8_t write_data[], int data_len);
uint8_t* i2c_read(char* DEV_I2C, int slave_addr, uint8_t read_addr[], int data_len, int read_len);

void spi_meter_init(void);
float spi_meter_read_energy(void);
int spi_meter_read_value(void);

uint8_t* set_led_bytes(int cmd, uint8_t data1[], uint8_t data2[], uint8_t data3[]);
void value_to_bin(int digit, uint8_t bit[]);
void qled_date(int state);
void qled_time(int state);
void qled_init(int mode);
void qled_alarm(int mode);
int count_digits(int number);
void charging_time(int seconds, int energy, bool state);
bool compareArrays(const uint8_t array1[], const uint8_t array2[], size_t length);
void clearArray(uint8_t array[], size_t length);
void can_send(void);
void can_read(void);
int can_test(void);
#endif

