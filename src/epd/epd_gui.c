

#include "epd.h"


/******************************************************************************/


// FB参数
int fb_w;
int fb_h;

#define FB_SIZE 4096
u8 fb_bw[FB_SIZE];
u8 fb_rr[FB_SIZE];


/******************************************************************************/


void draw_pixel(int x, int y, int color)
{
	int nx, ny;
	int rmode = scr_mode&0x03;
	
	if(rmode==0){
		nx = x;
		ny = y;
	}else if(rmode==1){
		nx = scr_w-1-y;
		ny = x;
	}else if(rmode==2){
		nx = scr_w-1-x;
		ny = scr_h-1-y;
	}else if(rmode==3){
		nx = y;
		ny = scr_h-1-x;
	}
	if(scr_mode&MIRROR_H){
		nx += scr_padding;
	}

	int byte_pos = ny*line_bytes+(nx>>3);
	int bit_mask = 0x80>>(nx&7);

	if(color!=WHITE){
		fb_bw[byte_pos] &= ~bit_mask;
	}
	if(scr_mode&EPD_BWR && color==RED){
		fb_rr[byte_pos] |=  bit_mask;
	}
}


void draw_hline(int y, int x1, int x2, int color)
{
	int x;
	if(x1>x2){
		x = x1;
		x1 = x2;
		x2 = x;
	}
	
	for(x=x1; x<=x2; x+=1){
		draw_pixel(x, y, color);
	}
}


void draw_vline(int x, int y1, int y2, int color)
{
	int y;
	if(y1>y2){
		x = y1;
		y1 = y2;
		y2 = x;
	}
	
	for(y=y1; y<=y2; y+=1){
		draw_pixel(x, y, color);
	}
}


void draw_rect(int x1, int y1, int x2, int y2, int color)
{
	draw_hline(y1, x1, x2, color);
	draw_hline(y2, x1, x2, color);
	draw_vline(x1, y1, y2, color);
	draw_vline(x2, y1, y2, color);
}


void draw_box(int x1, int y1, int x2, int y2, int color)
{
	int y;
	
	for(y=y1; y<=y2; y++){
		draw_hline(y, x1, x2, color);
	}
}

/******************************************************************************/


#include "sfont.h"
#include "font56.h"

u8 *current_font = (u8*)sfont;

int select_font(int id)
{
	if(id==0)
		current_font = (u8*)sfont;
	else
		current_font = (u8*)F_DSEG7_56;
	
	return 0;
}


static u8 *find_font(u8 *font, int ucs)
{
	int total = *(u16*)font;
	int i;

	for(i=0; i<total; i++){
		if(*(u16*)(font+2+i*4+0)==ucs){
			int offset = *(u16*)(font+2+i*4+2);
			//printk("  %04x at %04x\n", ucs, offset);
			return font+offset;
		}
	}

	return NULL;
}


int fb_draw_font(int x, int y, int ucs, int color)
{
	int r, c;
	u8 *font_data = find_font(current_font, ucs);
	if(font_data==NULL){
		printk("fb_draw %04x: not found!\n");
		font_data = find_font(current_font, '?');
	}

	int ft_adv = font_data[0];
	int ft_bw = font_data[1];
	int ft_bh = font_data[2];
	int ft_bx = (signed char)font_data[3];
	int ft_by = (signed char)font_data[4];
	int ft_lsize = (ft_bw+7)/8;
	font_data += 5;

	x += ft_bx;
	y += ft_by;

	for (r=0; r<ft_bh; r++) {
		for (c=0; c<ft_bw; c++) {
			int b = font_data[c>>3];
			int mask = 0x80>>(c%8);
			if(b&mask){
				draw_pixel(x+c, y, color);
			}
			mask >>= 1;
		}
		font_data += ft_lsize;
		y += 1;
	}

	return ft_adv;
}


static int utf8_to_ucs(char **ustr)
{
	u8 *str = (u8*)*ustr;
	int ucs = 0;

	if(*str==0){
		return 0;
	}else if(*str<0x80){
		*ustr = (char*)str+1;
		return *str;
	}else if(*str<0xe0){
		ucs = ((str[0]&0x1f)<<6) | (str[1]&0x3f);
		*ustr = (char*)str+2;
		return ucs;
	}else{
		ucs = ((str[0]&0x0f)<<12) | ((str[1]&0x3f)<<6) | (str[2]&0x3f);
		*ustr = (char*)str+3;
		return ucs;
	}
}


void draw_text(int x, int y, char *str, int color)
{
	int ch;

	while(1){
		ch = utf8_to_ucs(&str);
		if(ch==0)
			break;
		x += fb_draw_font(x, y, ch, color);
	}
}


/******************************************************************************/
#if 0
char *wday_str[] = {
	"一",
	"二",
	"三",
	"四",
	"五",
	"六",
	"日",
};


static int wday = 0;
void fb_test(void)
{
	memset(fb_bw, 0xff, scr_h*line_bytes);
	if(scr_mode&EPD_BWR){
		memset(fb_rr, 0x00, scr_h*line_bytes);
	}

	draw_rect(0, 0, fb_w-1, fb_h-1, BLACK);
	draw_rect(1, 1, fb_w-2, fb_h-2, BLACK);

#if 0
	for(int i=0; i<3; i++){
		int x = 8+i*38;
		int y = 30;
		draw_rect(x, y, x+29, y+29, BLACK);
		//draw_rect(x+1, y+1, x+29-1, y+29-1, BLACK);
		draw_box(x+2, y+2, x+29-2, y+29-2, i);
	}
#endif

	select_font(0);

	char tbuf[64];
	sprintk(tbuf, "%4d年%2d月%2d日 星期%s", 2025, 4, 29, wday_str[wday]);
	draw_text(15, 85, tbuf, BLACK);
	
	wday += 1;
	if(wday==7)
		wday = 0;

	select_font(1);
	sprintk(tbuf, "%02d:%02d", 2+wday, 30+wday);
	draw_text(12, 20, tbuf, BLACK);

	epd_screen_update();
}
#endif

/******************************************************************************/

// 新增的绘图函数，支持二次开发指南中的绘图指令

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

// 填充圆函数
void draw_filled_circle(int cx, int cy, int radius, int color) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x*x + y*y <= radius*radius) {
                draw_pixel(cx + x, cy + y, color);
            }
        }
    }
}

// 画矩形函数 - RECT指令 (已存在，这里是增强版)
void draw_rectangle(int x1, int y1, int x2, int y2, int color, int filled) {
    if (filled) {
        draw_box(x1, y1, x2, y2, color);
    } else {
        draw_rect(x1, y1, x2, y2, color);
    }
}

// 简单的图片显示函数 - IMG指令
// 这里实现一个简单的单色位图显示
void draw_image(int x, int y, int width, int height, const uint8_t* image_data, int color) {
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            int byte_index = (row * width + col) / 8;
            int bit_index = 7 - ((row * width + col) % 8);
            
            if (image_data[byte_index] & (1 << bit_index)) {
                draw_pixel(x + col, y + row, color);
            }
        }
    }
}

/******************************************************************************/



