# Keil编译指南 - HMCLOCK增强功能

## 概述
本文档说明如何在Keil uVision环境中编译包含新增功能的HMCLOCK固件。

## 新增功能模块
- **中文日历模块** (`src/features/chinese_calendar.c/h`)
- **倒数日功能** (`src/features/countdown.c/h`) 
- **显示模式管理** (`src/features/display_modes.c/h`)
- **功能配置头文件** (`src/features/features_config.h`)

## Keil项目配置

### 1. 添加源文件到项目
在Keil uVision项目中添加以下文件：
```
src/features/chinese_calendar.c
src/features/countdown.c  
src/features/display_modes.c
```

### 2. 包含路径设置
在项目设置中添加包含路径：
```
src/features
```

### 3. 预处理器定义
如果需要禁用某些功能，可以在项目设置中添加：
```
DISABLE_CHINESE_CALENDAR=1    // 禁用中文日历
DISABLE_COUNTDOWN=1           // 禁用倒数日功能  
DISABLE_DISPLAY_MODES=1       // 禁用显示模式
```

## 依赖关系

### EPD显示函数
新功能需要以下EPD显示函数（应在现有EPD驱动中实现）：
```c
void epd_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void epd_draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void epd_draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void epd_fill_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void epd_draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void epd_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void epd_draw_string(uint16_t x, uint16_t y, const char* str, uint16_t color);
void epd_clear_screen(uint16_t color);
```

### 标准库函数
确保以下标准库函数可用：
- `abs()` - 绝对值计算
- `sin()`, `cos()` - 三角函数（用于模拟时钟）
- `strlen()`, `strcpy()` - 字符串操作

## 内存使用

### Flash存储需求
- 中文日历数据表：约8KB
- 倒数日事件存储：约2KB  
- 显示模式代码：约4KB
- **总计额外Flash需求：约14KB**

### RAM使用
- 显示数据结构：约1KB
- 临时计算缓冲区：约512字节
- **总计额外RAM需求：约1.5KB**

## 编译错误解决

### 常见错误及解决方案

1. **类型定义冲突**
   ```
   error: typedef redefinition with different types
   ```
   **解决方案**：确保没有包含mock_sdk.h文件

2. **函数未声明**
   ```
   warning: implicit declaration of function
   ```
   **解决方案**：检查features_config.h是否正确包含

3. **链接错误**
   ```
   error: undefined reference to 'xxx'
   ```
   **解决方案**：确保所有.c文件都添加到项目中

## 功能验证

### 编译后验证步骤
1. 检查Flash使用量是否在限制内
2. 验证RAM使用量是否合理
3. 测试基本时钟显示功能
4. 验证BLE连接和配置功能

### 调试建议
- 使用串口输出调试信息
- 逐步启用功能模块
- 监控内存使用情况

## 注意事项

1. **兼容性**：新功能与原有代码完全兼容
2. **性能**：显示刷新可能略有延迟，属正常现象
3. **存储**：倒数日事件需要Flash存储支持
4. **配置**：Web界面配置需要BLE连接

## 技术支持

如遇到编译问题，请检查：
1. Dialog SDK版本兼容性
2. Keil编译器版本
3. 项目配置设置
4. 依赖函数实现

---
*最后更新：2025-05-29*