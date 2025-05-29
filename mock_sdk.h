/**
 * 模拟SDK头文件，用于编译测试
 */

#ifndef _MOCK_SDK_H_
#define _MOCK_SDK_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// 模拟SDK的基本定义
#define CFG_PRINTF
#define __RETAINED

// 模拟用户配置
#define CFG_USER_CONFIG

// 模拟基本类型
typedef uint8_t ke_msg_id_t;
typedef uint8_t ke_task_id_t;

// 模拟函数声明
void printk(const char *fmt, ...);

#endif // _MOCK_SDK_H_