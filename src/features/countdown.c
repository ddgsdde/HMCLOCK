/**
 ****************************************************************************************
 * @file countdown.c
 * @brief Countdown functionality implementation
 ****************************************************************************************
 */

#include "countdown.h"
#include <string.h>
#include <stdlib.h>

/*
 * LOCAL VARIABLES
 ****************************************************************************************
 */

static countdown_event_t countdown_events[MAX_COUNTDOWN_EVENTS];
static uint8_t event_count = 0;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void countdown_init(void)
{
    memset(countdown_events, 0, sizeof(countdown_events));
    event_count = 0;
    
    // 尝试从Flash加载保存的事件
    countdown_load_from_flash();
}

int countdown_add_event(const countdown_event_t *event)
{
    if (event_count >= MAX_COUNTDOWN_EVENTS) {
        return -1;
    }
    
    memcpy(&countdown_events[event_count], event, sizeof(countdown_event_t));
    countdown_events[event_count].enabled = 1;
    
    return event_count++;
}

int countdown_remove_event(uint8_t index)
{
    if (index >= event_count) {
        return -1;
    }
    
    // 移动后面的事件向前
    for (int i = index; i < event_count - 1; i++) {
        memcpy(&countdown_events[i], &countdown_events[i + 1], sizeof(countdown_event_t));
    }
    
    event_count--;
    memset(&countdown_events[event_count], 0, sizeof(countdown_event_t));
    
    return 0;
}

int countdown_update_event(uint8_t index, const countdown_event_t *event)
{
    if (index >= event_count) {
        return -1;
    }
    
    memcpy(&countdown_events[index], event, sizeof(countdown_event_t));
    return 0;
}

const countdown_event_t* countdown_get_event(uint8_t index)
{
    if (index >= event_count) {
        return NULL;
    }
    
    return &countdown_events[index];
}

static int is_leap_year(uint16_t year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int get_days_in_month(uint16_t year, uint8_t month)
{
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    
    return days_in_month[month - 1];
}

static int32_t date_to_days(uint16_t year, uint8_t month, uint8_t day)
{
    int32_t days = 0;
    int i;
    
    // 计算年份的天数
    for (i = 1; i < year; i++) {
        days += is_leap_year(i) ? 366 : 365;
    }
    
    // 计算月份的天数
    for (i = 1; i < month; i++) {
        days += get_days_in_month(year, i);
    }
    
    // 加上日期
    days += day;
    
    return days;
}

static void get_next_occurrence(const countdown_event_t *event, 
                               uint16_t current_year, uint8_t current_month, uint8_t current_day,
                               uint8_t current_hour, uint8_t current_minute, uint8_t current_second,
                               uint16_t *next_year, uint8_t *next_month, uint8_t *next_day,
                               uint8_t *next_hour, uint8_t *next_minute)
{
    *next_year = event->target_year;
    *next_month = event->target_month;
    *next_day = event->target_day;
    *next_hour = event->target_hour;
    *next_minute = event->target_minute;
    
    switch (event->type) {
        case COUNTDOWN_TYPE_ONCE:
            // 一次性事件，不需要调整
            break;
            
        case COUNTDOWN_TYPE_YEARLY:
            // 每年重复
            *next_year = current_year;
            
            // 如果今年的日期已过，则设为明年
            if (current_month > event->target_month ||
                (current_month == event->target_month && current_day > event->target_day) ||
                (current_month == event->target_month && current_day == event->target_day && 
                 current_hour > event->target_hour) ||
                (current_month == event->target_month && current_day == event->target_day && 
                 current_hour == event->target_hour && current_minute >= event->target_minute)) {
                *next_year = current_year + 1;
            }
            break;
            
        case COUNTDOWN_TYPE_MONTHLY:
            // 每月重复
            *next_year = current_year;
            *next_month = current_month;
            
            // 如果本月的日期已过，则设为下月
            if (current_day > event->target_day ||
                (current_day == event->target_day && current_hour > event->target_hour) ||
                (current_day == event->target_day && current_hour == event->target_hour && 
                 current_minute >= event->target_minute)) {
                *next_month = current_month + 1;
                if (*next_month > 12) {
                    *next_month = 1;
                    *next_year = current_year + 1;
                }
            }
            
            // 检查目标日期是否在下个月存在
            if (*next_day > get_days_in_month(*next_year, *next_month)) {
                *next_day = get_days_in_month(*next_year, *next_month);
            }
            break;
            
        case COUNTDOWN_TYPE_WEEKLY:
            // 每周重复 - 简化实现，这里需要更复杂的日期计算
            // 暂时按日处理
            *next_year = current_year;
            *next_month = current_month;
            *next_day = current_day + 7;
            
            // 处理月份边界
            while (*next_day > get_days_in_month(*next_year, *next_month)) {
                *next_day -= get_days_in_month(*next_year, *next_month);
                *next_month += 1;
                if (*next_month > 12) {
                    *next_month = 1;
                    *next_year += 1;
                }
            }
            break;
            
        case COUNTDOWN_TYPE_DAILY:
            // 每日重复
            *next_year = current_year;
            *next_month = current_month;
            *next_day = current_day;
            
            // 如果今天的时间已过，则设为明天
            if (current_hour > event->target_hour ||
                (current_hour == event->target_hour && current_minute >= event->target_minute)) {
                *next_day = current_day + 1;
                
                // 处理月份边界
                if (*next_day > get_days_in_month(*next_year, *next_month)) {
                    *next_day = 1;
                    *next_month = current_month + 1;
                    if (*next_month > 12) {
                        *next_month = 1;
                        *next_year = current_year + 1;
                    }
                }
            }
            break;
    }
}

void countdown_calculate(const countdown_event_t *event, 
                        uint16_t current_year, uint8_t current_month, uint8_t current_day,
                        uint8_t current_hour, uint8_t current_minute, uint8_t current_second,
                        countdown_result_t *result)
{
    uint16_t target_year, next_year;
    uint8_t target_month, target_day, target_hour, target_minute, next_month, next_day, next_hour, next_minute;
    
    // 获取下次发生的时间
    get_next_occurrence(event, current_year, current_month, current_day,
                       current_hour, current_minute, current_second,
                       &next_year, &next_month, &next_day, &next_hour, &next_minute);
    
    // 计算当前时间和目标时间的天数差
    int32_t current_days = date_to_days(current_year, current_month, current_day);
    int32_t target_days = date_to_days(next_year, next_month, next_day);
    
    int32_t total_minutes_current = current_days * 24 * 60 + current_hour * 60 + current_minute;
    int32_t total_minutes_target = target_days * 24 * 60 + next_hour * 60 + next_minute;
    
    int32_t diff_minutes = total_minutes_target - total_minutes_current;
    
    if (diff_minutes < 0) {
        result->is_past = 1;
        diff_minutes = -diff_minutes;
    } else {
        result->is_past = 0;
    }
    
    result->days = diff_minutes / (24 * 60);
    result->hours = (diff_minutes % (24 * 60)) / 60;
    result->minutes = diff_minutes % 60;
    result->seconds = 60 - current_second; // 简化处理
    
    if (result->seconds == 60) {
        result->seconds = 0;
    }
}

const countdown_event_t* countdown_get_next_event(uint16_t current_year, uint8_t current_month, uint8_t current_day,
                                                  uint8_t current_hour, uint8_t current_minute, uint8_t current_second)
{
    const countdown_event_t *next_event = NULL;
    countdown_result_t min_result;
    int32_t min_total_minutes = INT32_MAX;
    
    for (int i = 0; i < event_count; i++) {
        if (!countdown_events[i].enabled) {
            continue;
        }
        
        countdown_result_t result;
        countdown_calculate(&countdown_events[i], current_year, current_month, current_day,
                           current_hour, current_minute, current_second, &result);
        
        if (!result.is_past) {
            int32_t total_minutes = result.days * 24 * 60 + result.hours * 60 + result.minutes;
            if (total_minutes < min_total_minutes) {
                min_total_minutes = total_minutes;
                next_event = &countdown_events[i];
                min_result = result;
            }
        }
    }
    
    return next_event;
}

uint8_t countdown_get_event_count(void)
{
    return event_count;
}

int countdown_save_to_flash(void)
{
    // 这里应该实现Flash存储
    // 暂时返回成功
    return 0;
}

int countdown_load_from_flash(void)
{
    // 这里应该实现从Flash加载
    // 暂时返回成功
    return 0;
}