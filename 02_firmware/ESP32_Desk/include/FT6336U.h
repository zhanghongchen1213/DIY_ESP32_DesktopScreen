/**************************************************************************/
/*!
  @file     FT6336U.h
  Author: Atsushi Sasaki(https://github.com/aselectroworks)
  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _FT6336U_H
#define _FT6336U_H

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

#define I2C_ADDR_FT6336U 0x38

// Touch Parameter
#define FT6336U_PRES_DOWN 0x2
#define FT6336U_COORD_UD 0x1

// Registers
#define FT6336U_ADDR_DEVICE_MODE 0x00
typedef enum
{
    working_mode = 0b000,
    factory_mode = 0b100,
} DEVICE_MODE_Enum;
#define FT6336U_ADDR_GESTURE_ID 0x01
#define FT6336U_ADDR_TD_STATUS 0x02

#define FT6336U_ADDR_TOUCH1_EVENT 0x03
#define FT6336U_ADDR_TOUCH1_ID 0x05
#define FT6336U_ADDR_TOUCH1_X 0x03
#define FT6336U_ADDR_TOUCH1_Y 0x05
#define FT6336U_ADDR_TOUCH1_WEIGHT 0x07
#define FT6336U_ADDR_TOUCH1_MISC 0x08

#define FT6336U_ADDR_TOUCH2_EVENT 0x09
#define FT6336U_ADDR_TOUCH2_ID 0x0B
#define FT6336U_ADDR_TOUCH2_X 0x09
#define FT6336U_ADDR_TOUCH2_Y 0x0B
#define FT6336U_ADDR_TOUCH2_WEIGHT 0x0D
#define FT6336U_ADDR_TOUCH2_MISC 0x0E

#define FT6336U_ADDR_THRESHOLD 0x80
#define FT6336U_ADDR_FILTER_COE 0x85
#define FT6336U_ADDR_CTRL 0x86
typedef enum
{
    keep_active_mode = 0,
    switch_to_monitor_mode = 1,
} CTRL_MODE_Enum;
#define FT6336U_ADDR_TIME_ENTER_MONITOR 0x87
#define FT6336U_ADDR_ACTIVE_MODE_RATE 0x88
#define FT6336U_ADDR_MONITOR_MODE_RATE 0x89

#define FT6336U_ADDR_RADIAN_VALUE 0x91
#define FT6336U_ADDR_OFFSET_LEFT_RIGHT 0x92
#define FT6336U_ADDR_OFFSET_UP_DOWN 0x93
#define FT6336U_ADDR_DISTANCE_LEFT_RIGHT 0x94
#define FT6336U_ADDR_DISTANCE_UP_DOWN 0x95
#define FT6336U_ADDR_DISTANCE_ZOOM 0x96

#define FT6336U_ADDR_LIBRARY_VERSION_H 0xA1
#define FT6336U_ADDR_LIBRARY_VERSION_L 0xA2
#define FT6336U_ADDR_CHIP_ID 0xA3
#define FT6336U_ADDR_G_MODE 0xA4
typedef enum
{
    pollingMode = 0,
    triggerMode = 1,
} G_MODE_Enum;
#define FT6336U_ADDR_POWER_MODE 0xA5
#define FT6336U_ADDR_FIRMARE_ID 0xA6
#define FT6336U_ADDR_FOCALTECH_ID 0xA8
#define FT6336U_ADDR_RELEASE_CODE_ID 0xAF
#define FT6336U_ADDR_STATE 0xBC

// Function Specific Type
typedef enum
{
    touch = 0,
    stream,
    release,
} TouchStatusEnum;
typedef struct
{
    TouchStatusEnum status;
    uint16_t x;
    uint16_t y;
} TouchPointType;
typedef struct
{
    uint8_t touch_count;
    TouchPointType tp[2];
} FT6336U_TouchPointType;

// Uncomment to enable debug messages
// #define FT6336U_DEBUG

// Define where debug output will be printed
#define DEBUG_PRINTER Serial

// Setup debug printing macros
#ifdef FT6336U_DEBUG
#define DEBUG_PRINT(...)                  \
    {                                     \
        DEBUG_PRINTER.print(__VA_ARGS__); \
    }
#define DEBUG_PRINTLN(...)                  \
    {                                       \
        DEBUG_PRINTER.println(__VA_ARGS__); \
    }
#else
#define DEBUG_PRINT(...) \
    {                    \
    }
#define DEBUG_PRINTLN(...) \
    {                      \
    }
#endif

/**************************************************************************/
/*!
    @brief  FT6336U I2C CTP controller driver
*/
/**************************************************************************/
class FT6336U
{
public:
    FT6336U(uint8_t rst_n, uint8_t int_n);
#if defined(ESP32) || defined(ESP8266)
    FT6336U(int8_t sda, int8_t scl, uint8_t rst_n, uint8_t int_n);
#endif
    virtual ~FT6336U();

    void begin(void);

    uint8_t read_device_mode(void); // 读取或设置设备的工作模式（如工作模式或工厂模式）
    void write_device_mode(DEVICE_MODE_Enum);
    uint8_t read_gesture_id(void);   // 读取当前识别的手势ID，用于手势识别
    uint8_t read_td_status(void);    // 读取触摸点状态，判断是否有触摸发生
    uint8_t read_touch_number(void); // 读取当前触摸的点数
    /*读取第一个触摸点的X和Y坐标*/
    uint16_t read_touch1_x(void);
    uint16_t read_touch1_y(void);
    /*读取触摸点的详细信息，包括触摸事件类型、触摸ID、触摸力度和其他附加信息*/
    uint8_t read_touch1_event(void);
    uint8_t read_touch1_id(void);
    uint8_t read_touch1_weight(void);
    uint8_t read_touch1_misc(void);
    uint16_t read_touch2_x(void);
    uint16_t read_touch2_y(void);
    uint8_t read_touch2_event(void);
    uint8_t read_touch2_id(void);
    uint8_t read_touch2_weight(void);
    uint8_t read_touch2_misc(void);

    // Mode Parameter Register 读取和设置触摸阈值、过滤系数、控制模式
    uint8_t read_touch_threshold(void);    // 读取触摸阈值，决定了触摸识别的灵敏度
    uint8_t read_filter_coefficient(void); // 读取滤波系数，用于调整触摸信号的滤波强度
    /*获取或设置控制模式，比如保持活动模式或切换到监视模式*/
    uint8_t read_ctrl_mode(void);
    void write_ctrl_mode(CTRL_MODE_Enum mode);

    uint8_t read_time_period_enter_monitor(void); // 读取进入监视模式前的时间周期
    uint8_t read_active_rate(void);               // 读取活动模式下的报告率
    uint8_t read_monitor_rate(void);              // 读取监视模式下的报告率

    // Gestrue Parameter Register 读取或设置手势检测函数
    /*读取/设置弧度值：获取或设置手势移动的弧度值，用于调整手势识别的灵敏度。*/
    uint8_t read_radian_value(void);
    void write_radian_value(uint8_t val);
    /*读取/设置左右偏移：获取或设置左右滑动手势的偏移量，用于定义手势的识别范围。*/
    uint8_t read_offset_left_right(void);
    void write_offset_left_right(uint8_t val);
    /*读取/设置上下偏移：获取或设置上下滑动手势的偏移量，同样用于手势识别范围的定义。*/
    uint8_t read_offset_up_down(void);
    void write_offset_up_down(uint8_t val);
    /*读取/设置左右距离：获取或设置左右滑动的距离参数，影响手势识别的距离阈值。*/
    uint8_t read_distance_left_right(void);
    void write_distance_left_right(uint8_t val);
    /*读取/设置上下距离：获取或设置上下滑动的距离参数，同样影响手势识别的距离阈值。*/
    uint8_t read_distance_up_down(void);
    void write_distance_up_down(uint8_t val);
    /*读取/设置缩放距离：获取或设置缩放（如双指放大/缩小）手势的识别距离。*/
    uint8_t read_distance_zoom(void);
    void write_distance_zoom(uint8_t val);

    // System Information 读取库版本、芯片ID、固件ID和电源模式等设备信息。
    uint16_t read_library_version(void);
    uint8_t read_chip_id(void);
    uint8_t read_g_mode(void);
    void write_g_mode(G_MODE_Enum mode);
    uint8_t read_pwrmode(void);
    uint8_t read_firmware_id(void);
    uint8_t read_focaltech_id(void);
    uint8_t read_release_code_id(void);
    uint8_t read_state(void);

    // Scan Function
    FT6336U_TouchPointType scan(void);

private:
    int8_t sda = -1;
    int8_t scl = -1;
    uint8_t rst_n = -1;
    uint8_t int_n = -1;

    uint8_t readByte(uint8_t addr);
    void writeByte(uint8_t addr, uint8_t data);

    FT6336U_TouchPointType touchPoint;
};
#endif