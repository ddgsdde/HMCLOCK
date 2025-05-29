/**
 ****************************************************************************************
 * @file display_modes.h
 * @brief Display modes management for different UI styles
 ****************************************************************************************
 */

#ifndef _DISPLAY_MODES_H_
#define _DISPLAY_MODES_H_

#include <stdint.h>
#include <string.h>
#include <math.h>

// Forward declarations for EPD functions (actual implementations in EPD driver)
extern void epd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
extern void epd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
extern void epd_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
extern void epd_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
extern void epd_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
extern void epd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
extern void epd_draw_string(uint16_t x, uint16_t y, const char* str, uint16_t color);
extern void epd_clear_screen(uint16_t color);

// Color definitions
#define EPD_COLOR_BLACK     0x0000
#define EPD_COLOR_WHITE     0xFFFF
#define EPD_COLOR_RED       0xF800
#define EPD_COLOR_GREEN     0x07E0
#define EPD_COLOR_BLUE      0x001F
#define EPD_COLOR_YELLOW    0xFFE0
#define EPD_COLOR_CYAN      0x07FF
#define EPD_COLOR_MAGENTA   0xF81F
#include "chinese_calendar.h"
#include "countdown.h"

/*
 * DEFINES
 ****************************************************************************************
 */

#define MAX_IMAGE_SIZE          (32 * 1024)  // 32KB max image size
#define IMAGE_FLASH_ADDR        0x40000      // Flash address for images

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef enum {
    DISPLAY_MODE_TIME = 0,      // 时间模式
    DISPLAY_MODE_CALENDAR,      // 日历模式
    DISPLAY_MODE_IMAGE,         // 图片模式
    DISPLAY_MODE_COUNTDOWN,     // 倒数日模式
    DISPLAY_MODE_MAX
} display_mode_t;

typedef enum {
    TIME_STYLE_DIGITAL = 0,     // 数字时钟
    TIME_STYLE_ANALOG,          // 模拟时钟
    TIME_STYLE_MINIMAL,         // 极简风格
    TIME_STYLE_RETRO,           // 复古风格
    TIME_STYLE_NEON,            // 霓虹风格
    TIME_STYLE_MAX
} time_style_t;

typedef enum {
    CALENDAR_STYLE_GRID = 0,    // 网格日历
    CALENDAR_STYLE_LIST,        // 列表日历
    CALENDAR_STYLE_MINIMAL,     // 极简日历
    CALENDAR_STYLE_CHINESE,     // 中式日历
    CALENDAR_STYLE_MODERN,      // 现代风格
    CALENDAR_STYLE_MAX
} calendar_style_t;

typedef struct {
    display_mode_t current_mode;
    time_style_t time_style;
    calendar_style_t calendar_style;
    uint8_t show_seconds;           // 是否显示秒
    uint8_t show_date;              // 是否显示日期
    uint8_t show_lunar;             // 是否显示农历
    uint8_t show_festival;          // 是否显示节日
    uint8_t show_weather;           // 是否显示天气
    uint8_t show_countdown;         // 是否显示倒数日
    uint8_t auto_brightness;        // 自动亮度
    uint8_t brightness_level;       // 亮度等级 (1-10)
    uint8_t refresh_interval;       // 刷新间隔 (秒)
    uint8_t image_index;            // 当前图片索引
} display_config_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t weekday;
    chinese_date_t chinese_date;
    const countdown_event_t *next_countdown;
    countdown_result_t countdown_result;
    int16_t temperature;            // 温度 (摄氏度 * 10)
    uint8_t humidity;               // 湿度 (%)
    uint8_t battery_level;          // 电池电量 (%)
} display_data_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 * @brief Initialize display modes system
 */
void display_modes_init(void);

/**
 * @brief Set current display mode
 * @param[in] mode Display mode to set
 */
void display_set_mode(display_mode_t mode);

/**
 * @brief Get current display mode
 * @return Current display mode
 */
display_mode_t display_get_mode(void);

/**
 * @brief Set time display style
 * @param[in] style Time style to set
 */
void display_set_time_style(time_style_t style);

/**
 * @brief Set calendar display style
 * @param[in] style Calendar style to set
 */
void display_set_calendar_style(calendar_style_t style);

/**
 * @brief Update display configuration
 * @param[in] config New configuration
 */
void display_update_config(const display_config_t *config);

/**
 * @brief Get current display configuration
 * @return Pointer to current configuration
 */
const display_config_t* display_get_config(void);

/**
 * @brief Render current display mode
 * @param[in] data Display data to render
 */
void display_render(const display_data_t *data);

/**
 * @brief Render time mode
 * @param[in] data Display data
 * @param[in] style Time style
 */
void display_render_time(const display_data_t *data, time_style_t style);

/**
 * @brief Render calendar mode
 * @param[in] data Display data
 * @param[in] style Calendar style
 */
void display_render_calendar(const display_data_t *data, calendar_style_t style);

/**
 * @brief Render image mode
 * @param[in] image_index Image index to display
 */
void display_render_image(uint8_t image_index);

/**
 * @brief Render countdown mode
 * @param[in] data Display data
 */
void display_render_countdown(const display_data_t *data);

/**
 * @brief Upload image to flash
 * @param[in] image_data Image data
 * @param[in] size Image size
 * @param[in] index Image index
 * @return 0 if success, -1 if failed
 */
int display_upload_image(const uint8_t *image_data, uint32_t size, uint8_t index);

/**
 * @brief Get image from flash
 * @param[in] index Image index
 * @param[out] size Image size
 * @return Pointer to image data or NULL if not found
 */
const uint8_t* display_get_image(uint8_t index, uint32_t *size);

/**
 * @brief Save display configuration to flash
 * @return 0 if success, -1 if failed
 */
int display_save_config(void);

/**
 * @brief Load display configuration from flash
 * @return 0 if success, -1 if failed
 */
int display_load_config(void);

/**
 * @brief Switch to next display mode
 */
void display_next_mode(void);

/**
 * @brief Switch to next style within current mode
 */
void display_next_style(void);

#endif // _DISPLAY_MODES_H_