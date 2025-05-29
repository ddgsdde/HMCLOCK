/**
 ****************************************************************************************
 * @file display_modes.c
 * @brief Display modes implementation
 ****************************************************************************************
 */

#include "display_modes.h"
#include "../../mock_epd.h"
#include <string.h>
#include <stdio.h>

/*
 * LOCAL VARIABLES
 ****************************************************************************************
 */

static display_config_t display_config = {
    .current_mode = DISPLAY_MODE_TIME,
    .time_style = TIME_STYLE_DIGITAL,
    .calendar_style = CALENDAR_STYLE_GRID,
    .show_seconds = 1,
    .show_date = 1,
    .show_lunar = 1,
    .show_festival = 1,
    .show_weather = 0,
    .show_countdown = 1,
    .auto_brightness = 1,
    .brightness_level = 5,
    .refresh_interval = 60,
    .image_index = 0
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void display_modes_init(void)
{
    // 加载配置
    display_load_config();
}

void display_set_mode(display_mode_t mode)
{
    if (mode < DISPLAY_MODE_MAX) {
        display_config.current_mode = mode;
    }
}

display_mode_t display_get_mode(void)
{
    return display_config.current_mode;
}

void display_set_time_style(time_style_t style)
{
    if (style < TIME_STYLE_MAX) {
        display_config.time_style = style;
    }
}

void display_set_calendar_style(calendar_style_t style)
{
    if (style < CALENDAR_STYLE_MAX) {
        display_config.calendar_style = style;
    }
}

void display_update_config(const display_config_t *config)
{
    memcpy(&display_config, config, sizeof(display_config_t));
}

const display_config_t* display_get_config(void)
{
    return &display_config;
}

void display_render(const display_data_t *data)
{
    // 清空屏幕
    fb_clear();
    
    switch (display_config.current_mode) {
        case DISPLAY_MODE_TIME:
            display_render_time(data, display_config.time_style);
            break;
            
        case DISPLAY_MODE_CALENDAR:
            display_render_calendar(data, display_config.calendar_style);
            break;
            
        case DISPLAY_MODE_IMAGE:
            display_render_image(display_config.image_index);
            break;
            
        case DISPLAY_MODE_COUNTDOWN:
            display_render_countdown(data);
            break;
            
        default:
            display_render_time(data, TIME_STYLE_DIGITAL);
            break;
    }
    
    // 更新屏幕
    epd_update();
}

void display_render_time(const display_data_t *data, time_style_t style)
{
    char time_str[32];
    char date_str[64];
    char lunar_str[64];
    
    switch (style) {
        case TIME_STYLE_DIGITAL:
            // 数字时钟风格
            select_font(1); // 大字体
            
            if (display_config.show_seconds) {
                sprintf(time_str, "%02d:%02d:%02d", data->hour, data->minute, data->second);
            } else {
                sprintf(time_str, "%02d:%02d", data->hour, data->minute);
            }
            
            // 居中显示时间
            int time_width = fb_text_width(time_str);
            fb_text((scr_w - time_width) / 2, 60, time_str, BLACK);
            
            select_font(0); // 小字体
            
            if (display_config.show_date) {
                sprintf(date_str, "%04d年%02d月%02d日", data->year, data->month, data->day);
                int date_width = fb_text_width(date_str);
                fb_text((scr_w - date_width) / 2, 120, date_str, BLACK);
            }
            
            if (display_config.show_lunar) {
                sprintf(lunar_str, "%s年 %s%s", 
                       data->chinese_date.lunar_year_name,
                       data->chinese_date.lunar_month_name,
                       data->chinese_date.lunar_day_name);
                int lunar_width = fb_text_width(lunar_str);
                fb_text((scr_w - lunar_width) / 2, 140, lunar_str, BLACK);
            }
            
            if (display_config.show_festival && strlen(data->chinese_date.festival) > 0) {
                int festival_width = fb_text_width(data->chinese_date.festival);
                fb_text((scr_w - festival_width) / 2, 160, data->chinese_date.festival, RED);
            }
            break;
            
        case TIME_STYLE_ANALOG:
            // 模拟时钟风格
            display_render_analog_clock(data);
            break;
            
        case TIME_STYLE_MINIMAL:
            // 极简风格
            display_render_minimal_time(data);
            break;
            
        case TIME_STYLE_RETRO:
            // 复古风格
            display_render_retro_time(data);
            break;
            
        case TIME_STYLE_NEON:
            // 霓虹风格
            display_render_neon_time(data);
            break;
            
        default:
            // 默认数字风格
            display_render_time(data, TIME_STYLE_DIGITAL);
            break;
    }
    
    // 显示倒数日信息
    if (display_config.show_countdown && data->next_countdown) {
        char countdown_str[64];
        sprintf(countdown_str, "%s: %d天%d时%d分", 
               data->next_countdown->name,
               data->countdown_result.days,
               data->countdown_result.hours,
               data->countdown_result.minutes);
        fb_text(5, scr_h - 20, countdown_str, BLACK);
    }
}

void display_render_analog_clock(const display_data_t *data)
{
    int center_x = scr_w / 2;
    int center_y = scr_h / 2;
    int radius = (scr_w < scr_h ? scr_w : scr_h) / 2 - 20;
    
    // 画表盘
    draw_circle(center_x, center_y, radius);
    
    // 画刻度
    for (int i = 0; i < 12; i++) {
        float angle = i * 3.14159 / 6 - 3.14159 / 2;
        int x1 = center_x + (radius - 10) * cos(angle);
        int y1 = center_y + (radius - 10) * sin(angle);
        int x2 = center_x + radius * cos(angle);
        int y2 = center_y + radius * sin(angle);
        draw_line(x1, y1, x2, y2);
    }
    
    // 画时针
    float hour_angle = (data->hour % 12 + data->minute / 60.0) * 3.14159 / 6 - 3.14159 / 2;
    int hour_x = center_x + (radius * 0.5) * cos(hour_angle);
    int hour_y = center_y + (radius * 0.5) * sin(hour_angle);
    draw_thick_line(center_x, center_y, hour_x, hour_y, 3, BLACK);
    
    // 画分针
    float minute_angle = data->minute * 3.14159 / 30 - 3.14159 / 2;
    int minute_x = center_x + (radius * 0.7) * cos(minute_angle);
    int minute_y = center_y + (radius * 0.7) * sin(minute_angle);
    draw_thick_line(center_x, center_y, minute_x, minute_y, 2, BLACK);
    
    // 画秒针
    if (display_config.show_seconds) {
        float second_angle = data->second * 3.14159 / 30 - 3.14159 / 2;
        int second_x = center_x + (radius * 0.8) * cos(second_angle);
        int second_y = center_y + (radius * 0.8) * sin(second_angle);
        draw_line(center_x, center_y, second_x, second_y);
    }
    
    // 画中心点
    draw_circle(center_x, center_y, 3);
}

void display_render_minimal_time(const display_data_t *data)
{
    char time_str[16];
    
    select_font(1); // 大字体
    sprintf(time_str, "%02d:%02d", data->hour, data->minute);
    
    // 极简居中显示
    int time_width = fb_text_width(time_str);
    fb_text((scr_w - time_width) / 2, (scr_h - 40) / 2, time_str, BLACK);
    
    // 只显示日期
    if (display_config.show_date) {
        select_font(0);
        char date_str[32];
        sprintf(date_str, "%02d.%02d", data->month, data->day);
        int date_width = fb_text_width(date_str);
        fb_text((scr_w - date_width) / 2, (scr_h + 60) / 2, date_str, BLACK);
    }
}

void display_render_retro_time(const display_data_t *data)
{
    // 复古风格 - 添加装饰边框
    draw_rect(5, 5, scr_w - 10, scr_h - 10);
    draw_rect(10, 10, scr_w - 20, scr_h - 20);
    
    char time_str[32];
    select_font(1);
    
    if (display_config.show_seconds) {
        sprintf(time_str, "%02d:%02d:%02d", data->hour, data->minute, data->second);
    } else {
        sprintf(time_str, "%02d:%02d", data->hour, data->minute);
    }
    
    int time_width = fb_text_width(time_str);
    fb_text((scr_w - time_width) / 2, 50, time_str, BLACK);
    
    // 添加装饰线条
    draw_hline(80, 20, scr_w - 20, BLACK);
    draw_hline(90, 20, scr_w - 20, BLACK);
    
    select_font(0);
    if (display_config.show_date) {
        char date_str[64];
        sprintf(date_str, "%04d年%02d月%02d日", data->year, data->month, data->day);
        int date_width = fb_text_width(date_str);
        fb_text((scr_w - date_width) / 2, 110, date_str, BLACK);
    }
}

void display_render_neon_time(const display_data_t *data)
{
    // 霓虹风格 - 使用粗体和发光效果
    char time_str[32];
    select_font(1);
    
    sprintf(time_str, "%02d:%02d", data->hour, data->minute);
    
    int time_width = fb_text_width(time_str);
    int x = (scr_w - time_width) / 2;
    int y = 60;
    
    // 模拟发光效果 - 多次绘制
    fb_text(x-1, y-1, time_str, BLACK);
    fb_text(x+1, y-1, time_str, BLACK);
    fb_text(x-1, y+1, time_str, BLACK);
    fb_text(x+1, y+1, time_str, BLACK);
    fb_text(x, y, time_str, BLACK);
    
    // 添加装饰
    for (int i = 0; i < 5; i++) {
        draw_rect(x - 10 - i, y - 10 - i, time_width + 20 + 2*i, 40 + 2*i);
    }
}

void display_render_calendar(const display_data_t *data, calendar_style_t style)
{
    switch (style) {
        case CALENDAR_STYLE_GRID:
            display_render_grid_calendar(data);
            break;
            
        case CALENDAR_STYLE_LIST:
            display_render_list_calendar(data);
            break;
            
        case CALENDAR_STYLE_MINIMAL:
            display_render_minimal_calendar(data);
            break;
            
        case CALENDAR_STYLE_CHINESE:
            display_render_chinese_calendar(data);
            break;
            
        case CALENDAR_STYLE_MODERN:
            display_render_modern_calendar(data);
            break;
            
        default:
            display_render_grid_calendar(data);
            break;
    }
}

void display_render_grid_calendar(const display_data_t *data)
{
    char title[32];
    sprintf(title, "%04d年%02d月", data->year, data->month);
    
    select_font(0);
    int title_width = fb_text_width(title);
    fb_text((scr_w - title_width) / 2, 10, title, BLACK);
    
    // 星期标题
    const char* weekdays[] = {"日", "一", "二", "三", "四", "五", "六"};
    int cell_width = scr_w / 7;
    int cell_height = 20;
    
    for (int i = 0; i < 7; i++) {
        fb_text(i * cell_width + cell_width/2 - 5, 30, weekdays[i], BLACK);
    }
    
    // 画日历网格
    int days_in_month = get_days_in_month(data->year, data->month);
    int first_weekday = get_first_weekday(data->year, data->month);
    
    int row = 0;
    int col = first_weekday;
    
    for (int day = 1; day <= days_in_month; day++) {
        int x = col * cell_width;
        int y = 50 + row * cell_height;
        
        char day_str[4];
        sprintf(day_str, "%d", day);
        
        // 高亮今天
        if (day == data->day) {
            draw_box(x, y, x + cell_width - 1, y + cell_height - 1, BLACK);
            fb_text(x + cell_width/2 - 5, y + 5, day_str, WHITE);
        } else {
            fb_text(x + cell_width/2 - 5, y + 5, day_str, BLACK);
        }
        
        col++;
        if (col >= 7) {
            col = 0;
            row++;
        }
    }
}

void display_render_chinese_calendar(const display_data_t *data)
{
    char title[64];
    sprintf(title, "%s年 %s", 
           data->chinese_date.lunar_year_name,
           data->chinese_date.lunar_month_name);
    
    select_font(0);
    int title_width = fb_text_width(title);
    fb_text((scr_w - title_width) / 2, 10, title, BLACK);
    
    // 显示今日信息
    char today[64];
    sprintf(today, "今日: %s", data->chinese_date.lunar_day_name);
    int today_width = fb_text_width(today);
    fb_text((scr_w - today_width) / 2, 40, today, BLACK);
    
    // 显示节日
    if (strlen(data->chinese_date.festival) > 0) {
        int festival_width = fb_text_width(data->chinese_date.festival);
        fb_text((scr_w - festival_width) / 2, 70, data->chinese_date.festival, RED);
    }
    
    // 显示节气
    if (strlen(data->chinese_date.solar_term) > 0) {
        int term_width = fb_text_width(data->chinese_date.solar_term);
        fb_text((scr_w - term_width) / 2, 100, data->chinese_date.solar_term, BLACK);
    }
    
    // 显示阳历日期
    char solar[32];
    sprintf(solar, "公历: %04d年%02d月%02d日", data->year, data->month, data->day);
    int solar_width = fb_text_width(solar);
    fb_text((scr_w - solar_width) / 2, 130, solar, BLACK);
}

void display_render_image(uint8_t image_index)
{
    uint32_t size;
    const uint8_t *image_data = display_get_image(image_index, &size);
    
    if (image_data && size > 0) {
        // 这里应该实现图片解码和显示
        // 暂时显示占位符
        char placeholder[32];
        sprintf(placeholder, "图片 %d", image_index);
        select_font(1);
        int width = fb_text_width(placeholder);
        fb_text((scr_w - width) / 2, (scr_h - 20) / 2, placeholder, BLACK);
    } else {
        char no_image[] = "无图片";
        select_font(0);
        int width = fb_text_width(no_image);
        fb_text((scr_w - width) / 2, (scr_h - 10) / 2, no_image, BLACK);
    }
}

void display_render_countdown(const display_data_t *data)
{
    if (!data->next_countdown) {
        char no_event[] = "无倒数日事件";
        select_font(0);
        int width = fb_text_width(no_event);
        fb_text((scr_w - width) / 2, (scr_h - 10) / 2, no_event, BLACK);
        return;
    }
    
    // 显示事件名称
    select_font(0);
    int name_width = fb_text_width(data->next_countdown->name);
    fb_text((scr_w - name_width) / 2, 20, data->next_countdown->name, BLACK);
    
    // 显示倒数时间
    char countdown_str[64];
    select_font(1);
    
    if (data->countdown_result.days > 0) {
        sprintf(countdown_str, "%d天", data->countdown_result.days);
    } else if (data->countdown_result.hours > 0) {
        sprintf(countdown_str, "%d小时", data->countdown_result.hours);
    } else {
        sprintf(countdown_str, "%d分钟", data->countdown_result.minutes);
    }
    
    int countdown_width = fb_text_width(countdown_str);
    fb_text((scr_w - countdown_width) / 2, 80, countdown_str, BLACK);
    
    // 显示详细时间
    select_font(0);
    char detail_str[64];
    sprintf(detail_str, "%d天 %d时 %d分", 
           data->countdown_result.days,
           data->countdown_result.hours,
           data->countdown_result.minutes);
    int detail_width = fb_text_width(detail_str);
    fb_text((scr_w - detail_width) / 2, 120, detail_str, BLACK);
    
    // 显示事件描述
    if (strlen(data->next_countdown->desc) > 0) {
        int desc_width = fb_text_width(data->next_countdown->desc);
        fb_text((scr_w - desc_width) / 2, 150, data->next_countdown->desc, BLACK);
    }
}

int display_upload_image(const uint8_t *image_data, uint32_t size, uint8_t index)
{
    if (size > MAX_IMAGE_SIZE) {
        return -1;
    }
    
    // 这里应该实现Flash写入
    // 暂时返回成功
    return 0;
}

const uint8_t* display_get_image(uint8_t index, uint32_t *size)
{
    // 这里应该实现从Flash读取
    // 暂时返回NULL
    *size = 0;
    return NULL;
}

int display_save_config(void)
{
    // 这里应该实现配置保存到Flash
    return 0;
}

int display_load_config(void)
{
    // 这里应该实现从Flash加载配置
    return 0;
}

void display_next_mode(void)
{
    display_config.current_mode = (display_config.current_mode + 1) % DISPLAY_MODE_MAX;
}

void display_next_style(void)
{
    switch (display_config.current_mode) {
        case DISPLAY_MODE_TIME:
            display_config.time_style = (display_config.time_style + 1) % TIME_STYLE_MAX;
            break;
            
        case DISPLAY_MODE_CALENDAR:
            display_config.calendar_style = (display_config.calendar_style + 1) % CALENDAR_STYLE_MAX;
            break;
            
        default:
            break;
    }
}