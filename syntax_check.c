// 语法检查文件 - 包含我们新增的代码片段
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// 模拟定义
typedef uint8_t u8;
typedef enum { BLACK = 0, WHITE = 1 } color_t;
typedef enum { 
    DISPLAY_MODE_CLOCK = 0,
    DISPLAY_MODE_CALENDAR = 1,
    DISPLAY_MODE_IMAGE = 2,
    DISPLAY_MODE_CUSTOM = 3
} display_mode_t;

// 模拟全局变量
static display_mode_t current_display_mode = DISPLAY_MODE_CLOCK;
static int8_t timezone_offset = 8;
static char custom_text[1024] = {0};
static int screen_inverted = 0;
static int year = 2024, month = 4, date = 15, wday = 1;
static int hour = 12, minute = 30;
static char *wday_str[] = {"一", "二", "三", "四", "五", "六", "日"};
static int l_month = 4, l_date = 15;
static char *lday_str_hi[] = {"初", "十", "廿"};

// 模拟函数
void draw_pixel(int x, int y, int color) {}
void draw_text(int x, int y, char *str, int color) {}
void select_font(int id) {}
void epd_screen_update() {}
void printk(const char *fmt, ...) {}

// 变量替换函数
void replace_variables(char *text, char *output, int max_len) {
    int i = 0, j = 0;
    while (text[i] && j < max_len - 1) {
        if (text[i] == '{') {
            i++;
            if (text[i] == 'Y') {
                // 汉字年
                j += snprintf(output + j, max_len - j, "%d年", year);
            } else if (text[i] == 'y') {
                // 数字年
                j += snprintf(output + j, max_len - j, "%d", year);
            } else if (text[i] == 'M') {
                // 汉字月
                j += snprintf(output + j, max_len - j, "%d月", month + 1);
            } else if (text[i] == 'm') {
                // 数字月
                j += snprintf(output + j, max_len - j, "%02d", month + 1);
            } else if (text[i] == 'D') {
                // 汉字日
                j += snprintf(output + j, max_len - j, "%d日", date + 1);
            } else if (text[i] == 'd') {
                // 数字日
                j += snprintf(output + j, max_len - j, "%02d", date + 1);
            } else if (text[i] == 'W') {
                // 汉字星期
                char *wday_names[] = {"一", "二", "三", "四", "五", "六", "日"};
                j += snprintf(output + j, max_len - j, "星期%s", wday_names[wday]);
            } else if (text[i] == 'w') {
                // 数字星期
                j += snprintf(output + j, max_len - j, "%d", wday + 1);
            }
            i++; // 跳过变量名
            if (text[i] == '}') i++; // 跳过结束括号
        } else {
            output[j++] = text[i++];
        }
    }
    output[j] = '\0';
}

// 绘制自定义内容
void draw_custom_content() {
    char processed_text[1024];
    replace_variables(custom_text, processed_text, sizeof(processed_text));
    
    select_font(0);
    draw_text(10, 20, processed_text, screen_inverted ? WHITE : BLACK);
}

// 绘制日历模式
void draw_calendar_mode() {
    char tbuf[64];
    
    select_font(1);
    // 显示年月
    sprintf(tbuf, "%04d年%02d月", year, month + 1);
    draw_text(20, 20, tbuf, screen_inverted ? WHITE : BLACK);
    
    select_font(0);
    // 显示日期和星期
    sprintf(tbuf, "%02d日 星期%s", date + 1, wday_str[wday]);
    draw_text(20, 60, tbuf, screen_inverted ? WHITE : BLACK);
    
    // 显示农历
    sprintf(tbuf, "农历 %s月%s", 
            l_month > 12 ? "闰" : "",
            l_date < 10 ? lday_str_hi[0] : 
            l_date < 20 ? lday_str_hi[1] : lday_str_hi[2]);
    draw_text(20, 80, tbuf, screen_inverted ? WHITE : BLACK);
}

// 画点函数 - POINT指令
void draw_point(int x, int y, int color) {
    draw_pixel(x, y, color);
}

// 画线函数 - LINE指令
void draw_line(int x1, int y1, int x2, int y2, int color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    
    int x = x1, y = y1;
    
    while (1) {
        draw_pixel(x, y, color);
        
        if (x == x2 && y == y2) break;
        
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

// 画圆函数 - CIRCLE指令
void draw_circle(int cx, int cy, int radius, int color) {
    int x = 0;
    int y = radius;
    int d = 3 - 2 * radius;
    
    while (x <= y) {
        // 画8个对称点
        draw_pixel(cx + x, cy + y, color);
        draw_pixel(cx - x, cy + y, color);
        draw_pixel(cx + x, cy - y, color);
        draw_pixel(cx - x, cy - y, color);
        draw_pixel(cx + y, cy + x, color);
        draw_pixel(cx - y, cy + x, color);
        draw_pixel(cx + y, cy - x, color);
        draw_pixel(cx - y, cy - x, color);
        
        if (d < 0) {
            d = d + 4 * x + 6;
        } else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

int main() {
    printf("语法检查通过！\n");
    
    // 测试变量替换功能
    strcpy(custom_text, "今天是{Y}{M}{D} {W}");
    char output[1024];
    replace_variables(custom_text, output, sizeof(output));
    printf("变量替换测试: %s\n", output);
    
    return 0;
}