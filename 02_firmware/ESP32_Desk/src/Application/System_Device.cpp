#include "Application/System_Device.h"
#include "Application/System_Config.h"

/*任务句柄*/
TaskHandle_t Screen_Handle;
TaskHandle_t Touchpad_Handle;
TaskHandle_t Temp_Handle;

/*系统设备任务初始化*/
void Service_Devices_Init(void)
{
    xTaskCreatePinnedToCore(Device_Temp, "Dev.Temp", 1024 * 10, NULL, 3, &Temp_Handle, 0);
    xTaskCreatePinnedToCore(Device_Touchpad, "Dev.Touchpad", 1024 * 10, NULL, 6, &Touchpad_Handle, 0);
    xTaskCreatePinnedToCore(Device_Screen, "Dev.Screen", 1024 * 10, NULL, 5, &Screen_Handle, 0);
}

/*温湿度任务*/
void Device_Temp(void *arg)
{
    TickType_t _xTicksToWait = pdMS_TO_TICKS(60000); // 1min=60000
    while (1)
    {
        std::map<String, int> result = aht20.getTemp();
        Serial.println("当前湿度和温度为：");
        Serial.println(result["humidity"]);
        Serial.println(result["temperature"]);
        // mqtt.publish(result["temperature"], result["humidity"]);

        // 等待到下一次任务执行的时间
        vTaskDelay(_xTicksToWait);
    }
}

/*触摸检测任务*/
void Device_Touchpad(void *arg)
{
    TickType_t _xTicksToWait = pdMS_TO_TICKS(50); // 20ms
    while (1)
    {
        if (touchscreen.press())
        {
            std::map<String, uint16_t> result = touchscreen.read_date();
            Serial.println("接收触摸数据： ");
            // 打印result字典的全部键值对
            for (const auto &pair : result)
            {
                Serial.print(pair.first);
                Serial.print(": ");
                Serial.println(pair.second);
            }
            // 等待到下一次任务执行的时间
            vTaskDelay(_xTicksToWait);
        }
    }
}

/*屏幕刷新任务*/
void Device_Screen(void *arg)
{
    TickType_t _xTicksToWait = pdMS_TO_TICKS(150); // 150ms
    while (1)
    {
        screen.refresh();
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // 阻塞自身，等待通知后解除阻塞继续执行

        // 等待到下一次任务执行的时间
        vTaskDelay(_xTicksToWait);
    }
}