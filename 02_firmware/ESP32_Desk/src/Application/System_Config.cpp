#include "Application/System_Config.h"
#include "ui/ui.h"

#define WIFI_SSID "ZHC_Web"
#define WIFI_PASSWD "580231580231"

QueueHandle_t MusicCmd_Port;

/*创建设备*/
Network wifi;
MQTT mqtt;
AHT aht20;
Display screen;
Touch touchscreen;

/*系统资源初始化*/
void System_Resource_Init(void)
{
    MusicCmd_Port = xQueueCreate(4, sizeof(int8_t));

    // 联网上云
    wifi.init(WIFI_SSID, WIFI_PASSWD);
    // mqtt.connect();
    //mqtt.subscribe();

    // 初始化外设资源
    Serial.begin(9600);
    screen.init();
    aht20.init();
    touchscreen.init();

    // lvgl初始化
    lv_port_indev_init();

    ui_init();
    // 系统任务初始化
    System_Tasks_Init();
}

void System_Tasks_Init(void)
{
    // 系统级任务初始化
    Service_Devices_Init();     // 系统初始化
    Service_Interaction_Init(); // 数据交互初始化
}
