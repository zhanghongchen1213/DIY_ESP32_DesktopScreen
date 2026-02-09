#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "System_Device.h"
#include "System_Interaction.h"
#include "network.h"
#include "aht20.h"
#include "display.h"
#include "touch.h"

/* Used for freertos */
#define Tiny_Stack_Size 64
#define Small_Stack_Size 128
#define Normal_Stack_Size 256
#define Large_Stack_Size 512
#define Huge_Stack_Size 1024
#define PriorityVeryLow 1
#define PriorityLow 2
#define PriorityBelowNormal 3
#define PriorityNormal 4
#define PriorityAboveNormal 5
#define PriorityHigh 6
#define PrioritySuperHigh 7
#define PriorityRealtime 8


#ifdef __cplusplus
extern "C"
{
#endif
    /* RTOS resources */
    extern QueueHandle_t MusicCmd_Port;

#ifdef __cplusplus
} /*extern "C"*/
#endif

extern Network wifi;
extern MQTT mqtt;
extern AHT aht20;
extern Display screen;
extern Touch touchscreen;

void System_Tasks_Init(void);
void System_Resource_Init(void);

#endif