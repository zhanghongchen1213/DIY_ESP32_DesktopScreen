#include "display.h"

#ifndef MY_DISP_HOR_RES
#define MY_DISP_HOR_RES EPD_WIDTH // 表示电子墨水屏水平分辨率
#endif

#ifndef MY_DISP_VER_RES
#define MY_DISP_VER_RES EPD_HEIGHT // 表示电子墨水屏垂直分辨率
#endif

uint8_t UNCOLORED = 1; // 屏幕背景色
uint8_t COLORED = 0;   // 屏幕前景色

Epd epd;                                                // 电子墨水屏对象
uint8_t img_buf[MY_DISP_HOR_RES * MY_DISP_VER_RES / 8]; // 设置像素格式
Paint paint(img_buf, MY_DISP_HOR_RES, MY_DISP_VER_RES); // 在缓冲区中绘制图形和文本

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    for (int i = area->y1; i <= area->y2; i++)
    {
        for (int j = area->x1; j <= area->x2; j++)
        {
            if (color_p[(j - area->x1) + (i - area->y1) * w].full == 0xffff)
                paint.DrawPixel(j, i, UNCOLORED);
            else
                paint.DrawPixel(j, i, COLORED);
        }
    }
    lv_disp_flush_ready(disp_drv);
}

void Display::init()
{
    lv_init();

    /* 初始化电子屏 */
    if (epd.Init(lut_full_update) != 0)
    {
        Serial.println("e-Paper init failed");
        return;
    }
    else
    {
        Serial.println("e-Paper init success");
    }

    /* 设置画布属性 */
    paint.SetRotate(0);   // 旋转0
    paint.SetWidth(200);  // 宽度200
    paint.SetHeight(200); // 高度200
    paint.Clear(COLORED); // 清除颜色

    /* 清空屏幕*/
    epd.ClearFrameMemory(0xFF); // 清除电子墨水屏的帧内存，设置所有像素为白色
    epd.DisplayFrame();         // 显示帧内存中的图像，即清空
#if 0
    paint.DrawStringAt(50, 80, "ZHC", &Font24, UNCOLORED); // 参数可能需要调整
    // 从左向右，参数以此为：x坐标，y坐标，字符串，字体，颜色
    /* 将绘制的内容写入屏幕 */
    epd.SetFrameMemory(paint.GetImage());
    epd.DisplayFrame();

#endif

#if 1
    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
    static lv_disp_draw_buf_t draw_buf_dsc_1;
    static lv_color_t buf_1[MY_DISP_HOR_RES * 10];                             /*A buffer for 10 rows*/
    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * 10); /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/
    static lv_disp_drv_t disp_drv; /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);   /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
#endif
}

// 刷新电子屏显示
void Display::refresh(void)
{
    epd.SetFrameMemory(paint.GetImage()); // 设置电子屏的帧内存
    epd.DisplayFrame();                   // 显示图像
}

// 翻转电子屏颜色
void Display::flip(void)
{
    UNCOLORED = !UNCOLORED;
    COLORED = !COLORED;
}