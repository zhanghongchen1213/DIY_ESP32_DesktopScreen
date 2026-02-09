#ifndef _SERVICE_DEVICE_H
#define _SERVICE_DEVICE_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"


// C++条件编译
#ifdef __cplusplus
extern "C"
{
#endif
    extern TaskHandle_t Screen_Handle;
    extern TaskHandle_t Touchpad_Handle;
    extern TaskHandle_t Temp_Handle;

#ifdef __cplusplus
} /*extern "C"*/
#endif

void Service_Devices_Init(void);

void Device_Temp(void *arg);
void Device_Touchpad(void *arg);
void Device_Screen(void *arg);

#endif