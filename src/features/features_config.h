/**
 ****************************************************************************************
 * @file features_config.h
 * @brief Configuration header for new features integration
 ****************************************************************************************
 */

#ifndef _FEATURES_CONFIG_H_
#define _FEATURES_CONFIG_H_

// Enable/disable features
#define ENABLE_CHINESE_CALENDAR     1
#define ENABLE_COUNTDOWN            1
#define ENABLE_DISPLAY_MODES        1

// Include standard headers for real compilation
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Include feature headers
#if ENABLE_CHINESE_CALENDAR
#include "chinese_calendar.h"
#endif

#if ENABLE_COUNTDOWN
#include "countdown.h"
#endif

#if ENABLE_DISPLAY_MODES
#include "display_modes.h"
#endif

#endif // _FEATURES_CONFIG_H_