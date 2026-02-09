SET(SOURCES screens/ui_HomePage.c
    screens/ui_LovePage.c
    screens/ui_MenuPage.c
    screens/ui_TimePage.c
    screens/ui_WeatherPage.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_zhang_png.c
    images/ui_img_love_png.c
    images/ui_img_time_png.c
    images/ui_img_weather_png.c
    images/ui_img_home_png.c
    images/ui_img_back_png.c
    images/ui_img_cloudy_png.c
    images/ui_img_cloud_png.c
    images/ui_img_qr_png.c
    images/ui_img_rain_png.c
    images/ui_img_snowy_png.c
    images/ui_img_sunny_png.c
    fonts/ui_font_BG24s4bpp.c
    fonts/ui_font_BG30s4bpp.c
    fonts/ui_font_BG40s4bpp.c
    fonts/ui_font_LX34s4bpp.c
    fonts/ui_font_LX40s4bpp.c)

add_library(ui ${SOURCES})
