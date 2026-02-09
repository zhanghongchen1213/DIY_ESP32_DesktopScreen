#include "Application/System_Interaction.h"
#include "Application/System_Config.h"
#include "ui/ui.h"

/*任务句柄*/
TaskHandle_t updateTime_Handle;
TaskHandle_t updateInfo_handle;

/*系统任务初始化*/
void Service_Interaction_Init(void)
{
    xTaskCreatePinnedToCore(Interact_updateInfo, "Ineract.updateInfo", 1024 * 5, NULL, 2, &updateInfo_handle, 1);
    xTaskCreatePinnedToCore(Interact_updateTime, "Interact.updateTime", 1024 * 10, NULL, 4, &updateTime_Handle, 1);
}

/*天气更新任务*/
void Interact_updateInfo(void *arg)
{
    TickType_t _xTicksToWait = pdMS_TO_TICKS(3600000); // 1h=3600000
    while (1)
    {
        // 获取天气和温度
        std::map<String, String> result = wifi.getWeather(String("qingdao"));
        String weather = result["weather"];
        String temperature = result["temperature"] + "℃";
        String Weather_code = result["code"];
        int weatherCode = Weather_code.toInt();
        Serial.println("当前天气为：");
        Serial.println(weather);
        Serial.println("天气代码为： ");
        Serial.println(weatherCode);
        if (weatherCode >= 0 && weatherCode <= 1)
        {
            lv_img_set_src(ui_label_weather_pic, &ui_img_sunny_png);
            lv_label_set_text(ui_label_weather, "晴");
        }
        else if (weatherCode >= 4 && weatherCode <= 8)
        {
            lv_img_set_src(ui_label_weather_pic, &ui_img_cloud_png);
            lv_label_set_text(ui_label_weather, "云");
        }
        else if (weatherCode == 9)
        {
            lv_img_set_src(ui_label_weather_pic, &ui_img_cloudy_png);
            lv_label_set_text(ui_label_weather, "阴");
        }
        else if (weatherCode >= 10 && weatherCode <= 19)
        {
            lv_img_set_src(ui_label_weather_pic, &ui_img_rain_png);
            lv_label_set_text(ui_label_weather, "雨");
        }
        else if (weatherCode >= 20 && weatherCode <= 25)
        {
            lv_img_set_src(ui_label_weather_pic, &ui_img_snowy_png);
            lv_label_set_text(ui_label_weather, "雪");
        }
        else
        {
            lv_img_set_src(ui_label_weather_pic, &ui_img_sunny_png);
            lv_label_set_text(ui_label_weather, "晴");
        }
        lv_label_set_text(ui_label_temp, temperature.c_str());
        if (lv_scr_act() == ui_WeatherPage)
        {
            xTaskNotifyGive(Screen_Handle);
        }
        // 等待到下一次任务执行的时间
        vTaskDelay(_xTicksToWait);
    }
}

/*时间更新任务*/
void Interact_updateTime(void *arg)
{
    TickType_t _xTicksToWait = pdMS_TO_TICKS(30000); // 30s=30000
    struct tm timeinfo;
    char year_format[64];
    char time_format[64];
    char date_format[64];
    while (1)
    {
        vTaskDelay(_xTicksToWait);
        struct tm timeinfo;
        getLocalTime(&timeinfo);
        configTime(8 * 3600, 0, "ntp.aliyun.com");
        Serial.println(&timeinfo, "%F %T %A");
        strftime(year_format, sizeof(time_format), "%F", &timeinfo);
        strftime(time_format, sizeof(time_format), "%T", &timeinfo);
        strftime(date_format, sizeof(time_format), "%A", &timeinfo);
        String year(year_format);
        String time(time_format);
        String weekDay(date_format);
        Serial.println(weekDay);
        Serial.println(time);
        Serial.println(year);
        lv_label_set_text(ui_label_time, time.c_str());
        lv_label_set_text(ui_label_date, year.c_str());
        if (weekDay == "Monday")
            lv_label_set_text(ui_label_day, "周一");
        else if (weekDay == "Tueaday")
            lv_label_set_text(ui_label_day, "周二");
        else if (weekDay == "Wednesday")
            lv_label_set_text(ui_label_day, "周三");
        else if (weekDay == "Thursday")
            lv_label_set_text(ui_label_day, "周四");
        else if (weekDay == "Friday")
            lv_label_set_text(ui_label_day, "周五");
        else if (weekDay == "Saturday")
            lv_label_set_text(ui_label_day, "周六");
        else if (weekDay == "Sunday")
            lv_label_set_text(ui_label_day, "周日");

        /* Reflesh screen */
        if (lv_scr_act() == ui_TimePage)
        {
            xTaskNotifyGive(Screen_Handle);
        }
    }
}
