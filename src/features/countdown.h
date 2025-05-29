/**
 ****************************************************************************************
 * @file countdown.h
 * @brief Countdown functionality for important dates
 ****************************************************************************************
 */

#ifndef _COUNTDOWN_H_
#define _COUNTDOWN_H_

#include <stdint.h>
#include <string.h>

/*
 * DEFINES
 ****************************************************************************************
 */

#define MAX_COUNTDOWN_EVENTS    10
#define MAX_EVENT_NAME_LEN      32
#define MAX_EVENT_DESC_LEN      64

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef enum {
    COUNTDOWN_TYPE_ONCE = 0,        // 一次性事件
    COUNTDOWN_TYPE_YEARLY,          // 每年重复
    COUNTDOWN_TYPE_MONTHLY,         // 每月重复
    COUNTDOWN_TYPE_WEEKLY,          // 每周重复
    COUNTDOWN_TYPE_DAILY            // 每日重复
} countdown_type_t;

typedef struct {
    uint8_t enabled;                // 是否启用
    countdown_type_t type;          // 倒数类型
    uint16_t target_year;           // 目标年份
    uint8_t target_month;           // 目标月份
    uint8_t target_day;             // 目标日期
    uint8_t target_hour;            // 目标小时
    uint8_t target_minute;          // 目标分钟
    char name[MAX_EVENT_NAME_LEN];  // 事件名称
    char desc[MAX_EVENT_DESC_LEN];  // 事件描述
    uint8_t priority;               // 优先级 (1-10, 10最高)
    uint8_t show_on_main;           // 是否在主界面显示
} countdown_event_t;

typedef struct {
    int32_t days;                   // 剩余天数
    int32_t hours;                  // 剩余小时
    int32_t minutes;                // 剩余分钟
    int32_t seconds;                // 剩余秒数
    uint8_t is_past;                // 是否已过期
} countdown_result_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 * @brief Initialize countdown system
 */
void countdown_init(void);

/**
 * @brief Add a new countdown event
 * @param[in] event Event to add
 * @return Event index or -1 if failed
 */
int countdown_add_event(const countdown_event_t *event);

/**
 * @brief Remove a countdown event
 * @param[in] index Event index
 * @return 0 if success, -1 if failed
 */
int countdown_remove_event(uint8_t index);

/**
 * @brief Update a countdown event
 * @param[in] index Event index
 * @param[in] event Updated event data
 * @return 0 if success, -1 if failed
 */
int countdown_update_event(uint8_t index, const countdown_event_t *event);

/**
 * @brief Get countdown event by index
 * @param[in] index Event index
 * @return Pointer to event or NULL if not found
 */
const countdown_event_t* countdown_get_event(uint8_t index);

/**
 * @brief Calculate countdown for an event
 * @param[in] event Event to calculate
 * @param[in] current_year Current year
 * @param[in] current_month Current month
 * @param[in] current_day Current day
 * @param[in] current_hour Current hour
 * @param[in] current_minute Current minute
 * @param[in] current_second Current second
 * @param[out] result Countdown result
 */
void countdown_calculate(const countdown_event_t *event, 
                        uint16_t current_year, uint8_t current_month, uint8_t current_day,
                        uint8_t current_hour, uint8_t current_minute, uint8_t current_second,
                        countdown_result_t *result);

/**
 * @brief Get the most important upcoming event
 * @param[in] current_year Current year
 * @param[in] current_month Current month
 * @param[in] current_day Current day
 * @param[in] current_hour Current hour
 * @param[in] current_minute Current minute
 * @param[in] current_second Current second
 * @return Pointer to the most important event or NULL
 */
const countdown_event_t* countdown_get_next_event(uint16_t current_year, uint8_t current_month, uint8_t current_day,
                                                  uint8_t current_hour, uint8_t current_minute, uint8_t current_second);

/**
 * @brief Get number of active countdown events
 * @return Number of active events
 */
uint8_t countdown_get_event_count(void);

/**
 * @brief Save countdown events to flash
 * @return 0 if success, -1 if failed
 */
int countdown_save_to_flash(void);

/**
 * @brief Load countdown events from flash
 * @return 0 if success, -1 if failed
 */
int countdown_load_from_flash(void);

#endif // _COUNTDOWN_H_