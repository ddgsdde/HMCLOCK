/**
 * 集成测试文件 - 验证新功能模块的集成
 */

#include "src/features/chinese_calendar.h"
#include "src/features/countdown.h"
#include "src/features/display_modes.h"

// 模拟时间数据
void test_integration(void)
{
    // 测试中国日历
    chinese_date_t chinese_date;
    solar_to_lunar(2024, 5, 29, &chinese_date);
    
    // 测试倒计时
    countdown_event_t event;
    event.enabled = 1;
    event.type = COUNTDOWN_TYPE_ONCE;
    event.target_year = 2024;
    event.target_month = 6;
    event.target_day = 1;
    event.target_hour = 0;
    event.target_minute = 0;
    event.priority = 10; // 最高优先级
    event.show_on_main = 1;
    strcpy(event.name, "测试事件");
    strcpy(event.desc, "这是一个测试倒计时事件");
    
    countdown_add_event(&event);
    
    // 测试显示系统
    display_data_t display_data = {0};
    display_data.year = 2024;
    display_data.month = 5;
    display_data.day = 29;
    display_data.hour = 14;
    display_data.minute = 30;
    display_data.second = 45;
    display_data.chinese_date = chinese_date;
    
    // 测试不同显示模式
    display_render(&display_data);
}

int main(void)
{
    // 初始化模块
    chinese_calendar_init();
    countdown_init();
    display_modes_init();
    
    // 运行测试
    test_integration();
    
    return 0;
}