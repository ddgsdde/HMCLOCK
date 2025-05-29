/**
 ****************************************************************************************
 * @file chinese_calendar.c
 * @brief Chinese calendar and festivals implementation
 ****************************************************************************************
 */

#include "chinese_calendar.h"
#include <string.h>
#include <stdlib.h>

/*
 * LUNAR CALENDAR DATA
 ****************************************************************************************
 */

// 农历数据表 (1900-2100年)
// 每个数据包含该年的农历信息，包括闰月信息
static const uint32_t lunar_info[] = {
    0x04bd8,0x04ae0,0x0a570,0x054d5,0x0d260,0x0d950,0x16554,0x056a0,0x09ad0,0x055d2,
    0x04ae0,0x0a5b6,0x0a4d0,0x0d250,0x1d255,0x0b540,0x0d6a0,0x0ada2,0x095b0,0x14977,
    0x04970,0x0a4b0,0x0b4b5,0x06a50,0x06d40,0x1ab54,0x02b60,0x09570,0x052f2,0x04970,
    0x06566,0x0d4a0,0x0ea50,0x06e95,0x05ad0,0x02b60,0x186e3,0x092e0,0x1c8d7,0x0c950,
    0x0d4a0,0x1d8a6,0x0b550,0x056a0,0x1a5b4,0x025d0,0x092d0,0x0d2b2,0x0a950,0x0b557,
    0x06ca0,0x0b550,0x15355,0x04da0,0x0a5d0,0x14573,0x052d0,0x0a9a8,0x0e950,0x06aa0,
    0x0aea6,0x0ab50,0x04b60,0x0aae4,0x0a570,0x05260,0x0f263,0x0d950,0x05b57,0x056a0,
    0x096d0,0x04dd5,0x04ad0,0x0a4d0,0x0d4d4,0x0d250,0x0d558,0x0b540,0x0b5a0,0x195a6,
    0x095b0,0x049b0,0x0a974,0x0a4b0,0x0b27a,0x06a50,0x06d40,0x0af46,0x0ab60,0x09570,
    0x04af5,0x04970,0x064b0,0x074a3,0x0ea50,0x06b58,0x055c0,0x0ab60,0x096d5,0x092e0,
    0x0c960,0x0d954,0x0d4a0,0x0da50,0x07552,0x056a0,0x0abb7,0x025d0,0x092d0,0x0cab5,
    0x0a950,0x0b4a0,0x0baa4,0x0ad50,0x055d9,0x04ba0,0x0a5b0,0x15176,0x052b0,0x0a930,
    0x07954,0x06aa0,0x0ad50,0x05b52,0x04b60,0x0a6e6,0x0a4e0,0x0d260,0x0ea65,0x0d530,
    0x05aa0,0x076a3,0x096d0,0x04bd7,0x04ad0,0x0a4d0,0x1d0b6,0x0d250,0x0d520,0x0dd45,
    0x0b5a0,0x056d0,0x055b2,0x049b0,0x0a577,0x0a4b0,0x0aa50,0x1b255,0x06d20,0x0ada0,
    0x14b63,0x09370,0x049f8,0x04970,0x064b0,0x168a6,0x0ea50,0x06b20,0x1a6c4,0x0aae0,
    0x0a2e0,0x0d2e3,0x0c960,0x0d557,0x0d4a0,0x0da50,0x05d55,0x056a0,0x0a6d0,0x055d4,
    0x052d0,0x0a9b8,0x0a950,0x0b4a0,0x0b6a6,0x0ad50,0x055a0,0x0aba4,0x0a5b0,0x052b0,
    0x0b273,0x06930,0x07337,0x06aa0,0x0ad50,0x14b55,0x04b60,0x0a570,0x054e4,0x0d160,
    0x0e968,0x0d520,0x0daa0,0x16aa6,0x056d0,0x04ae0,0x0a9d4,0x0a2d0,0x0d150,0x0f252,
    0x0d520,0x0daa0,0x0b5a6,0x056d0,0x04ae0,0x0a9d4,0x0a2d0,0x0d150,0x0f252,0x0d520
};

// 生肖
static const char* zodiac_animals[] = {
    "鼠", "牛", "虎", "兔", "龙", "蛇", "马", "羊", "猴", "鸡", "狗", "猪"
};

// 农历月份
static const char* lunar_months[] = {
    "", "正月", "二月", "三月", "四月", "五月", "六月",
    "七月", "八月", "九月", "十月", "冬月", "腊月"
};

// 农历日期
static const char* lunar_days[] = {
    "", "初一", "初二", "初三", "初四", "初五", "初六", "初七", "初八", "初九", "初十",
    "十一", "十二", "十三", "十四", "十五", "十六", "十七", "十八", "十九", "二十",
    "廿一", "廿二", "廿三", "廿四", "廿五", "廿六", "廿七", "廿八", "廿九", "三十"
};

// 阳历节日
static const solar_festival_t solar_festivals[] = {
    {1, 1, "元旦"},
    {2, 14, "情人节"},
    {3, 8, "妇女节"},
    {3, 12, "植树节"},
    {4, 1, "愚人节"},
    {5, 1, "劳动节"},
    {5, 4, "青年节"},
    {6, 1, "儿童节"},
    {7, 1, "建党节"},
    {8, 1, "建军节"},
    {9, 10, "教师节"},
    {10, 1, "国庆节"},
    {12, 25, "圣诞节"},
    {0, 0, ""}  // 结束标记
};

// 农历节日
static const lunar_festival_t lunar_festivals[] = {
    {1, 1, "春节"},
    {1, 15, "元宵节"},
    {2, 2, "龙抬头"},
    {5, 5, "端午节"},
    {7, 7, "七夕节"},
    {7, 15, "中元节"},
    {8, 15, "中秋节"},
    {9, 9, "重阳节"},
    {12, 8, "腊八节"},
    {12, 23, "小年"},
    {0, 0, ""}  // 结束标记
};

// 二十四节气
static const solar_term_t solar_terms[] = {
    {1, 6, "小寒"}, {1, 20, "大寒"},
    {2, 4, "立春"}, {2, 19, "雨水"},
    {3, 6, "惊蛰"}, {3, 21, "春分"},
    {4, 5, "清明"}, {4, 20, "谷雨"},
    {5, 6, "立夏"}, {5, 21, "小满"},
    {6, 6, "芒种"}, {6, 22, "夏至"},
    {7, 7, "小暑"}, {7, 23, "大暑"},
    {8, 8, "立秋"}, {8, 23, "处暑"},
    {9, 8, "白露"}, {9, 23, "秋分"},
    {10, 8, "寒露"}, {10, 24, "霜降"},
    {11, 8, "立冬"}, {11, 22, "小雪"},
    {12, 7, "大雪"}, {12, 22, "冬至"},
    {0, 0, ""}  // 结束标记
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void chinese_calendar_init(void)
{
    // 初始化中国日历系统
    // 这里可以添加一些初始化代码
}

static int get_leap_month(uint16_t year)
{
    if (year < 1900 || year > 2100) return 0;
    return (lunar_info[year - 1900] & 0xf);
}

static int get_leap_days(uint16_t year)
{
    if (get_leap_month(year)) {
        return ((lunar_info[year - 1900] & 0x10000) ? 30 : 29);
    }
    return 0;
}

static int get_month_days(uint16_t year, uint8_t month)
{
    if (year < 1900 || year > 2100) return 30;
    return ((lunar_info[year - 1900] & (0x10000 >> month)) ? 30 : 29);
}

static int get_year_days(uint16_t year)
{
    int sum = 348;
    int i;
    
    if (year < 1900 || year > 2100) return 365;
    
    for (i = 0x8000; i > 0x8; i >>= 1) {
        sum += ((lunar_info[year - 1900] & i) ? 1 : 0);
    }
    return sum + get_leap_days(year);
}

void solar_to_lunar(uint16_t year, uint8_t month, uint8_t day, chinese_date_t *chinese_date)
{
    int i, leap = 0, temp = 0;
    int offset;
    
    // 计算距离1900年1月31日的天数
    offset = (year - 1900) * 365 + ((year - 1900) / 4) - ((year - 1900) / 100) + ((year - 1900) / 400);
    
    // 加上当年已过的天数
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        days_in_month[1] = 29;
    }
    
    for (i = 0; i < month - 1; i++) {
        offset += days_in_month[i];
    }
    offset += day - 31; // 减去1900年1月31日
    
    // 计算农历年份
    for (i = 1900; i < 2100 && offset > 0; i++) {
        temp = get_year_days(i);
        offset -= temp;
    }
    
    if (offset < 0) {
        offset += temp;
        i--;
    }
    
    chinese_date->year = year;
    chinese_date->month = month;
    chinese_date->day = day;
    chinese_date->lunar_year = i;
    
    // 计算农历月份
    leap = get_leap_month(i);
    int is_leap = 0;
    
    for (i = 1; i < 13 && offset > 0; i++) {
        if (leap > 0 && i == (leap + 1) && is_leap == 0) {
            --i;
            is_leap = 1;
            temp = get_leap_days(chinese_date->lunar_year);
        } else {
            temp = get_month_days(chinese_date->lunar_year, i);
        }
        
        if (is_leap == 1 && i == (leap + 1)) {
            is_leap = 0;
        }
        
        offset -= temp;
        if (offset <= 0) break;
    }
    
    if (offset == 0 && leap > 0 && i == leap + 1) {
        if (is_leap) {
            is_leap = 0;
        } else {
            is_leap = 1;
            --i;
        }
    }
    
    if (offset < 0) {
        offset += temp;
        --i;
    }
    
    chinese_date->lunar_month = i;
    chinese_date->lunar_day = offset + 1;
    chinese_date->is_leap_month = is_leap;
    
    // 设置生肖年
    strcpy(chinese_date->lunar_year_name, zodiac_animals[(chinese_date->lunar_year - 4) % 12]);
    
    // 设置农历月份名称
    if (is_leap) {
        strcpy(chinese_date->lunar_month_name, "闰");
        strcat(chinese_date->lunar_month_name, lunar_months[chinese_date->lunar_month]);
    } else {
        strcpy(chinese_date->lunar_month_name, lunar_months[chinese_date->lunar_month]);
    }
    
    // 设置农历日期名称
    strcpy(chinese_date->lunar_day_name, lunar_days[chinese_date->lunar_day]);
    
    // 获取节日信息
    const char* festival = get_festival_name(chinese_date);
    if (festival) {
        strcpy(chinese_date->festival, festival);
    } else {
        chinese_date->festival[0] = '\0';
    }
    
    // 获取节气信息
    const char* solar_term = get_solar_term(year, month, day);
    if (solar_term) {
        strcpy(chinese_date->solar_term, solar_term);
    } else {
        chinese_date->solar_term[0] = '\0';
    }
}

const char* get_festival_name(const chinese_date_t *chinese_date)
{
    int i;
    
    // 检查阳历节日
    for (i = 0; solar_festivals[i].month != 0; i++) {
        if (solar_festivals[i].month == chinese_date->month && 
            solar_festivals[i].day == chinese_date->day) {
            return solar_festivals[i].name;
        }
    }
    
    // 检查农历节日
    for (i = 0; lunar_festivals[i].month != 0; i++) {
        if (lunar_festivals[i].month == chinese_date->lunar_month && 
            lunar_festivals[i].day == chinese_date->lunar_day) {
            return lunar_festivals[i].name;
        }
    }
    
    return NULL;
}

const char* get_solar_term(uint16_t year, uint8_t month, uint8_t day)
{
    int i;
    
    for (i = 0; solar_terms[i].month != 0; i++) {
        if (solar_terms[i].month == month) {
            // 简化的节气计算，实际应该根据年份进行微调
            if (abs(solar_terms[i].day - day) <= 2) {
                return solar_terms[i].name;
            }
        }
    }
    
    return NULL;
}

const char* get_zodiac_animal(uint16_t year)
{
    return zodiac_animals[(year - 4) % 12];
}