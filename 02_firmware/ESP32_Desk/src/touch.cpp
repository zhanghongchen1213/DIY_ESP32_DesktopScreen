#include "touch.h"
Touch *globalTouchInstance = nullptr;

// 全局函数作为回调
void global_touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    if (globalTouchInstance != nullptr)
    {
        globalTouchInstance->touch_read(indev_driver, data);
    }
}

Touch::Touch(uint8_t ada, uint8_t scl, uint8_t rst, uint8_t intPin) : ft6336u(ada, scl, rst, intPin)
{
    globalTouchInstance = this;
}
void Touch::init(void)
{
    ft6336u.begin();
    Serial.print("FT6336U Firmware Version: ");
    Serial.println(ft6336u.read_firmware_id());
    Serial.print("FT6336U Device Mode: ");
    Serial.println(ft6336u.read_device_mode());
}

std::map<String, uint16_t> Touch ::read_date(void)
{
    std::map<String, uint16_t> result;
    result["status"] = ft6336u.read_td_status();
    result["touch_number"] = ft6336u.read_touch_number();
    result["x_1"] = ft6336u.read_touch1_x();
    result["y_1"] = ft6336u.read_touch1_y();
    result["touch1_event"] = ft6336u.read_touch1_event();
    result["x_2"] = ft6336u.read_touch2_x();
    result["y_2"] = ft6336u.read_touch2_y();
    result["touch2_event"] = ft6336u.read_touch1_event();

    return result;
}

void Touch::touch_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    if (ft6336u.read_td_status())
    {
        data->state = LV_INDEV_STATE_PR;
        data->point.x = ft6336u.read_touch1_x();
        data->point.y = ft6336u.read_touch1_y();
    }
    else
    {
        data->state = LV_INDEV_STATE_REL;
    }
}

bool Touch::double_click(void)
{
    return ft6336u.read_touch1_event() == 1 && ft6336u.read_touch2_event() == 1;
}

bool Touch::return_key(void)
{
    if ((ft6336u.read_touch1_x() < 25) && (ft6336u.read_touch1_y() < 35))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Touch::press(void)
{
    return ft6336u.read_td_status();
}

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER; // 表示输入设备类型为触摸屏
    indev_drv.read_cb = global_touch_read; // 使用全局函数作为回调
    lv_indev_drv_register(&indev_drv);      // 表示注册输入设备
}