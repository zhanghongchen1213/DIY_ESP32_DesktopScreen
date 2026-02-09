#ifndef _NETWORK_H
#define _NETWORK_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <map>

class Network
{
private:
    String User_ID = "ESP32_Desktop_Screen";         // 用户ID，必须唯一
    std::map<String, String> http_parseInfo(WiFiClient client); // http解析函数，解析心知天气返回的json数据
public:
    void init(String ssid, String password);          // 初始化网络
    std::map<String, String> getWeather(String city); // 获取天气,返回对应城市的天气和温度字典
    static void stringSplit(String *dst, String message, char flag); //分割字符串
};

class MQTT
{
private:
    String rootJson(float temp, float humi);
    void callback(char *topic, byte *payload, unsigned int length);
    void parseInfo(String message);

public:
    void connect();                                  // 连接EMQX服务器
    void publish(float temperature, float humidity); // 向MQTT平台发布主题
    void subscribe();
};

#endif
