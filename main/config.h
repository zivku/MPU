#ifndef CONFIG_H
#define CONFIG_H

#include "ini.h"
#include <cstring>
#include <stdlib.h> 
#include <stdio.h> 
typedef struct {
    double temperature_threshold;
    double max_voltage;
    int window_width;
    int window_hight;
    char* DEV_GPIO;  
    char* DEV_TTY;   
    char* DEV_TTY2;  
    char* DEV_SPI;
    char* SPI_METER;   
    char* PWM_export_PATH;
    char* PWM_period_PATH; 
    char* PWM_duty_cycle_PATH; 
    char* PWM_enable_PATH; 
    char* PWM_ADC_PATH;
    char* PWM_SET_ADC_PATH; 
    int GPIO_PIN_1;
    int GPIO_PIN_2;
    int SPEECH_PIN;
    int LED_NUM;
    int N_ERR;
    int L_ERR;
    int GFCI;
    char* GND_ERR; 
    int TIMEOUT_INIT;
    int TIMEOUT_6V;
    char* speech_path;
    int speech_addr;
    int over_voltage;
    int under_voltage;
    int over_temp;
    int over_current;
    char* qled_path;
    int flashing_time;
    int timeout_state;
    int stop_btn;
    int date_time;
    int main_delay;
    char* temp_adc;
    int no_voltage;
    int breathing_delay;
    int steady_delay;
    int flashing_delay;
    int test_delay;
    int light_hight;
    int light_down;
    char* lerr_adc;
    char* nerr_adc;
    int gfci_test;
    int gnderr_test_voltage;
    int gfci_test_hight_delay;
    int gfci_test_low_delay;
    int lerr_delta;
    int nerr_delta;
    int l_comparison;
    int n_comparison;
} Config;

extern Config config;
static int config_handler(void* user, const char* section, const char* name,
                          const char* value) {
    Config* config = (Config*)user;
    if (strcmp(section, "default") == 0) {
        if (strcmp(name, "window_width") == 0) {
            config->window_width = atoi(value);
        } 
        else if (strcmp(name, "window_hight") == 0) {
            config->window_hight = atoi(value);
        }
        else if (strcmp(name, "OVER_VOLTAGE") == 0) {
            config->over_voltage = atoi(value);
        }
        else if (strcmp(name, "UNDER_VOLTAGE") == 0) {
            config->under_voltage = atoi(value);
        }
        else if (strcmp(name, "NO_VOLTAGE") == 0) {
            config->no_voltage = atoi(value);
        }
        else if (strcmp(name, "BREATHING_DELAY") == 0) {
            config->breathing_delay = atoi(value);
        }
        else if (strcmp(name, "STEADY_DELAY") == 0) {
            config->steady_delay = atoi(value);
        }
        else if (strcmp(name, "FLASHING_DELAY") == 0) {
            config->flashing_delay = atoi(value);
        }
        else if (strcmp(name, "TEST_DELAY") == 0) {
            config->test_delay = atoi(value);
        }
        else if (strcmp(name, "LIGHT_HIGHT") == 0) {
            config->light_hight = atoi(value);
        }
        else if (strcmp(name, "LIGHT_DOWN") == 0) {
            config->light_down = atoi(value);
        }
        else if (strcmp(name, "GNDERR_TEST_VOLTAGE") == 0) {
            config->gnderr_test_voltage = atoi(value);
        }
        else if (strcmp(name, "GFCI_TEST_HIGHT_DELAY") == 0) {
            config->gfci_test_hight_delay = atoi(value);
        }
        else if (strcmp(name, "GFCI_TEST_LOW_DELAY") == 0) {
            config->gfci_test_low_delay = atoi(value);
        }
        else if (strcmp(name, "LERR_DELTA") == 0) {
            config->lerr_delta = atoi(value);
        }
        else if (strcmp(name, "NERR_DELTA") == 0) {
            config->nerr_delta = atoi(value);
        }
        else if (strcmp(name, "L_COMPARISON") == 0) {
            config->l_comparison = atoi(value);
        }
        else if (strcmp(name, "N_COMPARISON") == 0) {
            config->n_comparison = atoi(value);
        }
        else if (strcmp(name, "MAIN_DELAY") == 0) {
            config->main_delay = atoi(value);
        }
        else if (strcmp(name, "OVER_TEMP") == 0) {
            config->over_temp = atoi(value);
        }
        else if (strcmp(name, "OVER_CURRENT") == 0) {
            config->over_current = atoi(value);
        }
        else if (strcmp(name, "ICON_FLASHING_TIME") == 0) {
            config->flashing_time = atoi(value);
        }
        else if (strcmp(name, "STATE_TIMEOUT") == 0) {
            config->timeout_state = atoi(value);
        }
        else if (strcmp(name, "DATE_TIME") == 0) {
            config->date_time = atoi(value);
        }
        else if (strcmp(name, "DEV_GPIO") == 0) {
            config->DEV_GPIO = strdup(value);
            
        }
        else if (strcmp(name, "DEV_TTY") == 0) {
            config->DEV_TTY = strdup(value);
        }
        else if (strcmp(name, "DEV_TTY2") == 0) {
            config->DEV_TTY2 = strdup(value);
        }
        else if (strcmp(name, "QLED_UART") == 0) {
            config->qled_path = strdup(value);
        }
        else if (strcmp(name, "SPI_METER") == 0) {
            config->SPI_METER = strdup(value);
        }
        else if (strcmp(name, "I2C_PATH") == 0) {
            config->speech_path = strdup(value);
        }
        else if (strcmp(name, "SPEECH_ADDR") == 0) {
            config->speech_addr = atoi(value);
        }
        else if (strcmp(name, "DEV_SPI") == 0) {
            //config->DEV_SPI = strdup(value);
            char* cleaned_value = strdup(value);
            if (cleaned_value) {
                int length = strlen(cleaned_value);
                for (int i = 0; i < length; i++) {
                    if (cleaned_value[i] <= ' ') {
                        cleaned_value[i] = '\0';
                        break;
                    }
                }
                config->DEV_SPI = strdup(cleaned_value);
                free(cleaned_value);  // 释放清理后的字符串
            } else {
                fprintf(stderr, "Failed to allocate memory for DEV_SPI.\n");
                exit(1);
            }
        }
        else if (strcmp(name, "GPIO_PIN_1") == 0) {
            config->GPIO_PIN_1 = atoi(value);
        }
        else if (strcmp(name, "GPIO_PIN_2") == 0) {
            config->GPIO_PIN_2 = atoi(value);
        }
        else if (strcmp(name, "GFCI_TEST") == 0) {
            config->gfci_test = atoi(value);
        }
        else if (strcmp(name, "STOP_BTN") == 0) {
            config->stop_btn = atoi(value);
        }
        else if (strcmp(name, "SPEECH_PIN") == 0) {
            config->SPEECH_PIN = atoi(value);
        }
        else if (strcmp(name, "PWM_export_PATH") == 0) {
            config->PWM_export_PATH = strdup(value);
        }
        else if (strcmp(name, "PWM_period_PATH") == 0) {
            config->PWM_period_PATH = strdup(value);
        }
        else if (strcmp(name, "PWM_duty_cycle_PATH") == 0) {
            config->PWM_duty_cycle_PATH = strdup(value);
        }
        else if (strcmp(name, "PWM_enable_PATH") == 0) {
            config->PWM_enable_PATH = strdup(value);
        }
        else if (strcmp(name, "PWM_ADC_PATH") == 0) {
            config->PWM_ADC_PATH = strdup(value);
        }
        else if (strcmp(name, "TEMP_ADC") == 0) {
            config->temp_adc = strdup(value);
        }
        else if (strcmp(name, "LERR_ADC") == 0) {
            config->lerr_adc = strdup(value);
        }
        else if (strcmp(name, "NERR_ADC") == 0) {
            config->nerr_adc = strdup(value);
        }
        else if (strcmp(name, "PWM_SET_ADC_PATH") == 0) {
            config->PWM_SET_ADC_PATH = strdup(value);
        }
        else if (strcmp(name, "LED_NUM") == 0) {
            config->LED_NUM = atoi(value);
        }
        else if (strcmp(name, "L_ERR") == 0) {
            config->L_ERR = atoi(value);
        }
        else if (strcmp(name, "N_ERR") == 0) {
            config->N_ERR = atoi(value);
        }
        else if (strcmp(name, "GFCI") == 0) {
            config->GFCI = atoi(value);
        }
        else if (strcmp(name, "GND_ERR") == 0) {
            config->GND_ERR = strdup(value);
        }
        else if (strcmp(name, "TIMEOUT_INIT") == 0) {
            config->TIMEOUT_INIT = atoi(value);
        }
        else if (strcmp(name, "TIMEOUT_6V") == 0) {
            config->TIMEOUT_6V = atoi(value);
        }
    }
    return 1;
}

void load_config(const char* filename) {
    if (ini_parse(filename, config_handler, &config) < 0) {
        printf("Could not load config file %s\n", filename);
        exit(1);
    }
}

#endif