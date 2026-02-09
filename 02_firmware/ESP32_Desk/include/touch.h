#ifndef __TOUCH_H
#define __TOUCH_H

#include <Arduino.h>
#include "FT6336U.h"
#include <map>

#ifdef __cplusplus
extern "C"
{
#endif
    #include "lvgl.h"
    void lv_port_indev_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif


#define I2C_SDA 33
#define I2C_SCL 32
#define RST_N_PIN 5
#define INT_N_PIN 4

class Touch
{
private:
    FT6336U ft6336u;

public:
    Touch(uint8_t ada = I2C_SDA, uint8_t scl = I2C_SCL, uint8_t rst = RST_N_PIN, uint8_t intPin = INT_N_PIN);
    void init(void);
    std::map<String, uint16_t> read_date(void);
    void touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
    bool double_click(void);
    bool return_key(void);
    bool press(void);
};

#endif
