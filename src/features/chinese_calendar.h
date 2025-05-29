/**
 ****************************************************************************************
 * @file chinese_calendar.h
 * @brief Chinese calendar and festivals implementation
 ****************************************************************************************
 */

#ifndef _CHINESE_CALENDAR_H_
#define _CHINESE_CALENDAR_H_

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/*
 * DEFINES
 ****************************************************************************************
 */

#define MAX_FESTIVAL_NAME_LEN   16
#define MAX_SOLAR_TERMS_LEN     8

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

typedef struct {
    uint8_t month;
    uint8_t day;
    char name[MAX_FESTIVAL_NAME_LEN];
} solar_festival_t;

typedef struct {
    uint8_t month;
    uint8_t day;
    char name[MAX_FESTIVAL_NAME_LEN];
} lunar_festival_t;

typedef struct {
    uint8_t month;
    uint8_t day;
    char name[MAX_SOLAR_TERMS_LEN];
} solar_term_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t lunar_year;
    uint8_t lunar_month;
    uint8_t lunar_day;
    uint8_t is_leap_month;
    char lunar_year_name[8];    // 生肖年
    char lunar_month_name[8];   // 农历月份
    char lunar_day_name[8];     // 农历日期
    char solar_term[MAX_SOLAR_TERMS_LEN];
    char festival[MAX_FESTIVAL_NAME_LEN];
} chinese_date_t;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 * @brief Initialize Chinese calendar system
 */
void chinese_calendar_init(void);

/**
 * @brief Convert solar date to lunar date
 * @param[in] year Solar year
 * @param[in] month Solar month
 * @param[in] day Solar day
 * @param[out] chinese_date Chinese date information
 */
void solar_to_lunar(uint16_t year, uint8_t month, uint8_t day, chinese_date_t *chinese_date);

/**
 * @brief Get festival name for given date
 * @param[in] chinese_date Chinese date information
 * @return Festival name or NULL if no festival
 */
const char* get_festival_name(const chinese_date_t *chinese_date);

/**
 * @brief Get solar term for given date
 * @param[in] year Solar year
 * @param[in] month Solar month
 * @param[in] day Solar day
 * @return Solar term name or NULL if no solar term
 */
const char* get_solar_term(uint16_t year, uint8_t month, uint8_t day);

/**
 * @brief Get zodiac animal name for given year
 * @param[in] year Solar year
 * @return Zodiac animal name
 */
const char* get_zodiac_animal(uint16_t year);

#endif // _CHINESE_CALENDAR_H_