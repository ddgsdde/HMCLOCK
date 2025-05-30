

#include "epd.h"


/******************************************************************************/

// 硬件参数
int scr_w;
int scr_h;
int scr_mode;
int line_bytes;
int scr_padding;
int update_mode;
int lut_size;


// 窗口参数
int win_w;
int win_h;


/******************************************************************************/


u8 lut_fast_70[80] = {
//  RP0   RP1   RP2   RP3   RP4   RP5   RP6
    0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,  // LUT0
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00,  // LUT1
    0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,  // LUT0
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00,  // LUT1

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT4

    0x03, 0x03, 0x00, 0x00, 0x02,  // Group0
    0x09, 0x09, 0x00, 0x00, 0x02,  // Group1
    0x03, 0x03, 0x00, 0x00, 0x02,  // Group2
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group3
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group4
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group5
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group6

//	VGH   VSH1  VSH2  VSL   FR1   FR2
    0x15, 0x41, 0xa8, 0x32, 0x30, 0x0a,
};


// 这里如果B-B和W->W不驱动的话，屏幕很快就会花掉。
// 但如果驱动的话，这种黑白转换就没有意义了。
u8 lut_fly_70[80] = {
//  RP0   RP1   RP2   RP3   RP4   RP5   RP6
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT0  B->B
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT1  B->W
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT2  W->B
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT3  W->W

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT4

    0x0f, 0x00, 0x00, 0x00, 0x01,  // Group0
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group1
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group2
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group3
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group4
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group5
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group6

//	VGH   VSH1  VSH2  VSL   FR1   FR2
    0x15, 0x41, 0xa8, 0x32, 0x0a, 0x2e,
};


u8 lut_fast_100[112] = {
//  RP0   RP1   RP2   RP3   RP4   RP5   RP6   RP7   RP8   RP9
    0x80, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT0
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT1
    0x80, 0x60, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT0
    0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT1

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT4

    0x03, 0x03, 0x00, 0x00, 0x02,  // Group0
    0x06, 0x06, 0x00, 0x00, 0x02,  // Group1
    0x03, 0x03, 0x00, 0x00, 0x02,  // Group2
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group3
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group4
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group5
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group6
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group7
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group8
    0x00, 0x00, 0x00, 0x00, 0x00,  // Group9

//	VGH   VSH1  VSH2  VSL   VCOM  FR1   FR2
    0x15, 0x41, 0xa8, 0x32, 0x50, 0x0f, 0x0c,
};


// 这里如果B-B和W->W不驱动的话，屏幕很快就会花掉。
// 但如果驱动的话，这种黑白转换就没有意义了。
u8 lut_fly_100[112] = {
//  RP0   RP1   RP2   RP3   RP4   RP5   RP6   RP7   RP8   RP9
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT0  B->B
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT1  B->W
    0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT2  W->B
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT3  W->W

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // LUT4

    0x0f, 0x00, 0x00, 0x00, 0x01,  // Group0
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group1
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group2
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group3
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group4
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group5
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group6
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group7
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group8
    0x00, 0x00, 0x00, 0x00, 0x01,  // Group9

//	VGH   VSH1  VSH2  VSL   VCOM  FR1   FR2
    0x15, 0x41, 0xa8, 0x32, 0x50, 0x0f, 0x0c,
};

u8 *lut_fast = lut_fast_70;
u8 *lut_fly  = lut_fly_70;


/******************************************************************************/


// x必须是8的倍数，且x2>x1, Y2>y1
void epd_window(int x1, int y1, int x2, int y2)
{
	int tmp;

	x1 = x1&~0x07;
	x2 = x2&~0x07;
	win_w = x2-x1+8;
	win_h = y2-y1+1;

	int rmode = scr_mode&3;
	if(rmode==0 || rmode==2){
		fb_w = win_w;
		fb_h = win_h;
	}else{
		fb_w = win_h;
		fb_h = win_w;
	}

	//printk("epd_window: win: %d x %d  fb: %d x %d\n", win_w, win_h, fb_w, fb_h);
	//printk("  [%d-%d] [%d-%d]\n", x1, x2, y1, y2);

	if(scr_mode&MIRROR_H){
		tmp = x1;
		x1 = x2;
		x2 = tmp;
	}
	if(scr_mode&MIRROR_V){
		tmp = y1;
		y1 = y2;
		y2 = tmp;
	}
	

	int xinc = (x2>x1)? 1: 0;
	int yinc = (y2>y1)? 1: 0;

	epd_cmd2(0x0f, y1-1, (y1-1)>>8);
	epd_cmd3(0x01, y2-1, (y2-1)>>8, 0x00);  // Gate Driver output control

	epd_cmd(0x11);  // data entry mode
	epd_data((yinc<<1)|xinc);

	epd_cmd(0x44);  // set Ram-X address start/end position
	epd_data(x1>>3);
	epd_data(x2>>3);

	epd_cmd(0x45);  // set Ram-Y address start/end position
	epd_data(y1&0xff);
	epd_data(y1>>8);
	epd_data(y2&0xff);
	epd_data(y2>>8);

	epd_cmd(0x4e);  // set RAM-x address
	epd_data(x1>>3);
	epd_cmd(0x4f);  // set RAM-y address
	epd_data(y1&0xff);
	epd_data(y1>>8);

}


void epd_pos(int x, int y)
{
	epd_cmd(0x4e);  // set RAM-x address to 0
	epd_data(x);
	epd_cmd(0x4f);  // set RAM-y address to 249
	epd_data(y&0xff);
	epd_data(y>>8);
}



void epd_load_lut(u8 *lut)
{
	epd_cmd(0x32);
	epd_data_array(lut, lut_size);

	epd_cmd1(0x03, lut[lut_size+0]);
	epd_cmd3(0x04, lut[lut_size+1], lut[lut_size+2], lut[lut_size+3]);

	if(lut_size==70){
		epd_cmd1(0x3a, lut[lut_size+4]); // FR1
		epd_cmd1(0x3b, lut[lut_size+5]); // FR2
	}else{
		epd_cmd1(0x2c, lut[lut_size+4]); // VCOM
		epd_cmd1(0x3a, lut[lut_size+5]); // FR1
		epd_cmd1(0x3b, lut[lut_size+6]); // FR2
	}
}


void epd_dump_lut(void)
{
	u8 lut[256];
	int i, j;
	int group = (lut_size==70)? 7 : 10;

	epd_cmd_read(0x33, lut, 256);

	for(i=0; i<5; i++){
		printk("\t");
		for(j=0; j<group; j++){
			printk("0x%02x, ", lut[i*7+j]);
		}
		printk(" // LUT%d\n", i);
	}

	for(i=0; i<group; i++){
		printk("\t");
		for(j=0; j<5; j++){
			printk("0x%02x, ", lut[group*5+i*5+j]);
		}
		printk(" // Group%d\n", i);
	}

	printk("\t");
	for(i=0; i<7; i++){
		printk("0x%02x, ", lut[group*10+i]);
	}
	printk("\n\n");

	//hex_dump("LUT", lut, 256);
}


int epd_lut_size(void)
{
	u8 lut[256];
	int i;
	
	memset(lut, 0xa5, 256);
	epd_cmd(0x32);
	epd_data_array(lut, 256);

	memset(lut, 0x00, 256);
	epd_cmd_read(0x33, lut, 256);
	
	for(i=0; i<256; i++){
		if(lut[i]!=0xa5)
			break;
	}
	lut_size = i;
	printk("LUT size: %d\n", lut_size);
	//hex_dump("LUT", lut, 256);

	if(lut_size==100){
		lut_fast = lut_fast_100;
		lut_fly  = lut_fly_100;
	}

	return lut_size;
}


void epd_init(void)
{
	//printk("epd_init: %dx%d\n", scr_w, scr_h);

	epd_power(1);
	epd_reset(1);

#if 0
	epd_wait();
	epd_cmd(0x12);  // SWRESET
	delay_ms(10);
	epd_wait();

//	epd_lut_size();
#endif

	epd_cmd1(0x74, 0x54);
	epd_cmd1(0x7e, 0x3b);

	epd_cmd2(0x2b, 0x04, 0x63);
	epd_cmd4(0x0c, 0x8b, 0x9c, 0x96, 0x0f);

	epd_cmd3(0x01, scr_h-1, (scr_h-1)>>8, 0x00);  // Gate Driver output control
	epd_cmd1(0x11, 0x03);  // data entry mode
	epd_cmd1(0x3c, 0x05);  // border wavefrom

	epd_window(0, 0, scr_w-1, scr_h-1);

	epd_cmd1(0x18, 0x80);  // Read Built-in temperature sensor
}


void epd_update_mode(int mode)
{
	update_mode = mode;
}


void epd_update(void)
{
	int seq;

	if(update_mode==UPDATE_FULL){
		seq = 0xf7;
	}else{
		if(update_mode==UPDATE_FAST){
			epd_load_lut(lut_fast);
		}else{
			epd_load_lut(lut_fly);
		}
		seq = 0xc7;
	}

	epd_cmd1(0x22, seq);
	epd_cmd(0x20);
}


void epd_sleep(void)
{
	epd_cmd1(0x10, 01);
}


/******************************************************************************/

void epd_screen_update(void)
{
	int i;

	epd_cmd(0x24);  // write RAM for black(0)/white(1)
	for(i=0; i<win_h*line_bytes; i++){
		epd_data(fb_bw[i]);
	}
	if(scr_mode&EPD_BWR){
		epd_cmd(0x26);
		for(i=0; i<win_h*line_bytes; i++){
			epd_data(fb_rr[i]);
		}
	}
}


void epd_screen_clean(int mode)
{
	int x, y;

	epd_cmd(0x24);  // write RAM for black(0)/white(1)
	for(y=0; y<win_h; y++){
		for(x=0; x<win_w; x+=8){
			int d;
			if(mode==2){
				if(y<(win_h/2)){
					d = (x<win_w/2)? 0x77 : 0x00;
				}else{
					d = (x<win_w/2)? 0x00 : 0xff;
				}
			}else if(mode==1){
				d = 0xff;
			}else{
				d = 0x00;
			}
			epd_data(d);
		}
	}

	if(scr_mode&EPD_BWR){
		epd_cmd(0x26);  // write RAM for red(1)/other(0)
		for(y=0; y<win_h; y++){
			for(x=0; x<win_w; x+=8){
				int d = 0x00;
				if(mode==2){
					if(y<(win_h/2)){
						d = 0x00;
					}else{
						d = (x<win_w/2)? 0xff : 0x00;
					}
				}
				epd_data(d);
			}
		}
	}
}


/******************************************************************************/


int epd_detect(void)
{
	int retv = 0;

	epd_hw_open();
	epd_reset(1);
	epd_cmd(0x12); // SWRESET
	if(epd_busy()){
		epd_wait();
		epd_lut_size();
		retv = 1;
	}
	epd_hw_close();
	return retv;
}


/******************************************************************************/


#if 0

void epd_test(void)
{
	epd_hw_init(0x23200700, 0x05210006, 104, 212,         ROTATE_3);  // for 2.13 board BW
//	epd_hw_init(0x23111000, 0x07210120, 122, 250, EPD_BWR|ROTATE_3);  // for 2.13 board BWR
//	epd_hw_init(0x23200700, 0x05210006, 128, 296, EPD_BWR|ROTATE_3);  // for 2.90 board

	epd_init();

#if 0
	epd_screen_clean(2);
#else
	fb_test();
#endif
	epd_update();
	epd_wait();


#if 1
	epd_update_mode(UPDATE_FLY);
//	epd_cmd1(0x2c, 0x55);
//	epd_update(0xc0);

	for(int i=0; i<10; i++)
	{
		printk("%d\n", i);
		delay_ms(3000);
		fb_test();
		epd_update();
		epd_wait();
	}
#endif

	delay_ms(5000);
	printk("\nepd_test done.\n");
	epd_cmd1(0x10, 0x01);
	epd_power(0);
	epd_hw_close();
	//epd_update(0x03);
}

void epd_test2(void)
{

	epd_init(104, 212, EPD_BWR|ROTATE_3);

	epd_load_lut(lut);
//	epd_load_lut(lut_p);
	epd_cmd1(0x2c, 0x55);
	epd_update(0xc0);

	fb_test();
	epd_update(0x04);

	epd_cmd1(0x10, 0x03);
	epd_power(0);
}
#endif
