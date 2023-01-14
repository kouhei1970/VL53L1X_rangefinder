#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "VL53L1X_api.h"
#include "VL53L1X_calibration.h"

#define I2C_SDA_PIN 16
#define I2C_SCL_PIN 17

#define TEST_TIMEOUT_US 10000000

void i2c_master_init(void);
int continuous_reading(void);
int calibration(void);
void display_menu();

void main(void)
{
    int status;
    int answer_at_least_once=0;

    stdio_init_all();
    i2c_master_init();

    printf("Waiting to allow user to connect USB serial\n");
    sleep_ms(3000);
    printf("End waiting\n");

    while(1){
        int keycode;
        display_menu();
        
        do{
            stdio_flush();
            keycode = getchar_timeout_us(TEST_TIMEOUT_US);
            stdio_flush();
            if(!answer_at_least_once){
                display_menu();
            }
        }while(keycode == PICO_ERROR_TIMEOUT || keycode == 0);
        switch (keycode)
        {
        case 'o':
        case 'O':
            while(calibration());
            answer_at_least_once=1;
            break;
        
        case 'r':
        case 'R':
            answer_at_least_once=1;
            while(continuous_reading());
            break;

        }
    }

}

void display_menu(){
    printf("Select action :\n");
    printf("O - Offset Calibration\n");
    printf("R - Read distance\n");
}

int calibration(){
    uint16_t offset;
    int status;
    uint8_t boot_state=0;
    printf("Calibration...\n");
    while(!boot_state){
        VL53L1X_BootState(1, &boot_state);
    }
    printf("Sensor boot ok\n");

    status=VL53L1X_SensorInit(1);
    if(status){
        printf("Sensor Init KO, error %d\n", status);
    }else{
        printf("Sensor Init OK\n");
    }

    status = VL53L1X_CalibrateOffset(1, 140, &offset);
    if(status != 0){
        printf("Error while calibrating : %d\n",status);
    }else{
        printf("Offset : %d\n", offset);

    }
    return 0;
}

int continuous_reading(){
    int status;
    uint8_t data_ready, boot_state=0;
    uint16_t distance;

    printf("Reading distance...\nSend any character to quit.");

    while(!boot_state){
        VL53L1X_BootState(1, &boot_state);
    }
    printf("Sensor boot ok\n");

    status=VL53L1X_SensorInit(1);
    if(status){
        printf("Sensor Init KO, error %d\n", status);
        return 0;
    }else{
        printf("Sensor Init OK\n");
    }


    // Custom configuration
    status = VL53L1X_SetDistanceMode (1, 1); // Short mode
    status |= VL53L1X_SetInterMeasurementInMs(1, 200); 
    status |= VL53L1X_SetTimingBudgetInMs(1, 200);
    if(status){
        printf("Custom config KO, error %d\n", status);
        return 0;
    }else{
        printf("Custom config OK\n");
    }

    status=VL53L1X_StartRanging(1);
    if(status){
        printf("Start ranging KO, error %d\n", status);
        return 0;
    }else{
        printf("Start ranging OK\n");
    }

    while(1){
    // Reading data
        data_ready = 0;
        while(!data_ready){
            status=VL53L1X_CheckForDataReady(1, &data_ready);
            if(status){
                printf("CheckForDataReady KO, error %d\n", status);
                return 0;
            }else{
                //printf("CheckForDataReady OK\n");
            }
        }
        
        status=VL53L1X_GetDistance(1, &distance);
        if(status){
            printf("GetDistance KO, error %d\n", status);
            return 0;
        }else{
            //printf("GetDistance OK, distance %u mm\n", distance);
            printf(">distance:%d\n", distance);
        }
        
        status=VL53L1X_ClearInterrupt(1);
        if(status){
            printf("ClearInterrupt KO, error %d\n", status);
            return 0;
        }else{
            //printf("ClearInterrupt OK\n");
        }

        int lettre = getchar_timeout_us(0);
        if(lettre != PICO_ERROR_TIMEOUT && lettre != 0){
            return 0;
        }
    }
    return 0;
}

void i2c_master_init(void){
    //stdio_init_all();
    i2c_init(i2c0, 100 * 1000);

    printf("Initialisation des broches\n");
    for(int i=0; i++; i<=28){
        if(gpio_get_function(i) == GPIO_FUNC_I2C){
            printf("Pin I2C : %d\n", i);
            gpio_set_function(i, GPIO_FUNC_NULL);
        }
    }

    printf("%d and %d for I2C\n", I2C_SDA_PIN, I2C_SCL_PIN);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);

}
