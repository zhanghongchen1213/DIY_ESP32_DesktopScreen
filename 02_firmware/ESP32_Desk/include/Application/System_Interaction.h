#ifndef _SERVICE_INTERACTION_H
#define _SERVICE_INTERACTION_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"


#ifdef __cplusplus
extern "C"
{
#endif

    extern TaskHandle_t updateInfo_handle;
    extern TaskHandle_t updateTime_Handle;

#ifdef __cplusplus
} /*extern "C"*/
#endif

void Service_Interaction_Init(void);

void Interact_updateInfo(void *arg);
void Interact_updateTime(void *arg);
#endif