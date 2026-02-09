#include "network.h"

// MQTT服务器
#define mqtt_server "www.zhcmqtt.top"            // EMQX服务器的域名
#define publish_topic "ESP32_Desktop_Screen/pub" // 发布消息主题
#define subscribe_topic "Wechat/sub"             // 订阅消息主题

// 心知天气HTTP请求参数
String reqUserKey = "S3I1NnSIFREG0m3at"; // 私钥
String reqUnit = "c";                    // 摄氏
const char *http_host = "api.seniverse.com";
const int http_port = 80;

WiFiClient ESP32_Mqtt_Client;
WiFiClient ESP32_Http_Client;
PubSubClient ESP32_MQTT(ESP32_Mqtt_Client);

/*连接WIFI*/
void Network::init(String ssid, String password)
{
    Serial.println("Connecting to WiFi network: " + String(ssid));
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
};

/*获取天气,返回对应城市的天气和温度字典*/
std::map<String, String> Network::getWeather(String city)
{
    String reqRes = "/v3/weather/now.json?key=" + reqUserKey + "&location=" + city + "&language=en&unit=" + reqUnit;
    String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                         "Host: " + http_port + "\r\n" +
                         "Connection: close\r\n" + "\r\n";
    // 尝试连接服务器
    if (ESP32_Http_Client.connect(http_host, http_port))
    {
        // 向服务器发送http请求信息
        ESP32_Http_Client.print(httpRequest);
#if Debug
        Serial.println("Sending request: "); // 通过串口输出HTTP请求信息内容以便查阅
        Serial.println(httpRequest);
#endif
        // 获取并显示服务器响应状态行
        String status_response = ESP32_Http_Client.readStringUntil('\n');

#if Debug
        Serial.print("status_response: ");
        Serial.println(status_response);
#endif
        // 使用find跳过HTTP响应头
        if (ESP32_Http_Client.find("\r\n\r\n"))
        {
#if Debug
            Serial.println("Found Header End. Start Parsing.");
#endif
        }
        // 利用ArduinoJson库解析心知天气响应信息
        std::map<String, String> resultDict = http_parseInfo(ESP32_Http_Client);
        ESP32_Http_Client.stop();
        return resultDict;
    }
    else
    {
        Serial.println("connection failed!");
        ESP32_Http_Client.stop();
    }
};

/*解析解析心知天气响应信息*/
std::map<String, String> Network::http_parseInfo(WiFiClient client)
{
    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
    DynamicJsonDocument doc(capacity);

    deserializeJson(doc, client); // 将服务器响应信息反序列化为JSON对象

    JsonObject results_0 = doc["results"][0];

    JsonObject results_0_now = results_0["now"];
    const char *results_0_now_text = results_0_now["text"];               // "Sunny"
    const char *results_0_now_code = results_0_now["code"];               // "0"
    const char *results_0_now_temperature = results_0_now["temperature"]; // "32"

    const char *results_0_last_update = results_0["last_update"]; // "2020-06-02T14:40:00+08:00"

    // 通过串口监视器显示以上信息
    String results_0_now_text_str = results_0_now["text"].as<String>();
    String results_0_now_code_str = results_0_now["code"].as<String>();
    String results_0_now_temperature_str = results_0_now["temperature"].as<String>();

    String results_0_last_update_str = results_0["last_update"].as<String>();

    Serial.println(F("======Weahter Now======="));
    Serial.print(F("Weather Now: "));
    Serial.print(results_0_now_text_str);
    Serial.print(F(" "));
    Serial.println(results_0_now_code_str);
    Serial.print(F("Temperature: "));
    Serial.println(results_0_now_temperature_str);
    Serial.print(F("Last Update: "));
    Serial.println(results_0_last_update_str);
    Serial.println(F("========================"));
    // 创建字典存储结果
    std::map<String, String> result;
    result["weather"] = results_0_now_text_str;
    result["code"] = results_0_now_code_str;
    result["temperature"] = results_0_now_temperature_str;

    return result;
}

/*用于分割字符串*/
void Network::stringSplit(String* dst, String message, char flag){
    int flag_position;
    uint8_t num = 0;
    /* Split String */
    do {
        flag_position = message.indexOf(flag);
        if (flag_position != -1) {
            dst[num++] = message.substring(0, flag_position);
            message = message.substring(flag_position + 1, message.length());
        } else {
            dst[num] = message;
        }
    } while (flag_position >= 0);
}



/*连接EMQX服务器*/
void MQTT::connect()
{
    ESP32_MQTT.setServer(mqtt_server, 1883);
    while (!ESP32_MQTT.connected())
    {
        if (ESP32_MQTT.connect("ESP32_Desktop_Screen"))
        {
            Serial.println("MQTT connected");
        }
        else
        {
            Serial.print("failed with state ");
            Serial.print(ESP32_MQTT.state());
            delay(1000);
        }
    }
};

/*向MQTT平台发布主题*/
void MQTT::publish(float temperature, float humidity)
{
    char jsonBuf[192];
    sprintf(jsonBuf, rootJson(temperature, humidity).c_str());
    ESP32_MQTT.publish(publish_topic, jsonBuf);
    Serial.println("发布主题: ");
    Serial.println(publish_topic);
    Serial.println("发布主题消息内容: ");
    Serial.println(jsonBuf);
};

/*订阅主题*/
void MQTT::subscribe()
{
    ESP32_MQTT.subscribe(subscribe_topic);
};

void MQTT::callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("订阅主题: ");
    Serial.println(topic);
    String message;
    for (int i = 0; i < length; i++)
    {
        message = message + (char)payload[i]; // convert *byte to string
    }
    Serial.print("订阅主题消息内容: ");
    Serial.print(message);
    // 解析json数据
    parseInfo(message);
};

/*解析订阅主题信息*/
void MQTT::parseInfo(String message)
{
    // Stream& input;

    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, message);

    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }
};

/*打包温湿度数据*/
String MQTT::rootJson(float temp, float humi)
{
    StaticJsonDocument<192> doc;

    JsonObject info = doc.createNestedObject("info");
    info["name"] = "ESP32_Desktop_Screen";
    info["time"] = "2024-1-27";

    JsonObject parameter = doc.createNestedObject("parameter");
    parameter["temperature"] = String(temp);
    parameter["humidity"] = String(humi);
    String jsonCode;
    serializeJson(doc, jsonCode);
    return jsonCode;
};