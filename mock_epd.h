/**
 * 模拟EPD头文件，用于编译测试
 */

#ifndef _MOCK_EPD_H_
#define _MOCK_EPD_H_

#include "mock_sdk.h"
#include <math.h>

// 颜色定义
#define BLACK 0
#define WHITE 1
#define RED   2

// 模拟EPD相关定义
extern uint8_t *fb_bw;
extern uint8_t *fb_rr;
extern int scr_w, scr_h, line_bytes;

// 模拟字体相关
typedef struct {
    int width;
    int height;
    const uint8_t *data;
} font_t;

extern const font_t font_16;
extern const font_t font_24;
extern const font_t font_32;

// 模拟绘图函数
void draw_text(int x, int y, const char *text, const font_t *font);
void draw_line(int x1, int y1, int x2, int y2);
void draw_rect(int x, int y, int w, int h);
void draw_circle(int x, int y, int r);
void draw_hline(int x, int y, int w, int color);
void draw_box(int x1, int y1, int x2, int y2, int color);
void draw_thick_line(int x1, int y1, int x2, int y2, int thickness, int color);

// 模拟显示函数
void fb_clear(void);
void fb_text(int x, int y, const char *text, int color);
int fb_text_width(const char *text);
void select_font(int size);
void epd_update(void);

// 模拟日期函数
int get_days_in_month(int year, int month);
int get_first_weekday(int year, int month);

#endif // _MOCK_EPD_H_