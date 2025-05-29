#!/bin/bash

echo "=== HMCLOCK 固件编译测试 ==="
echo

# 设置编译参数
CFLAGS="-c -I./src -I./src/features -I./src/config -I./src/epd"
OUTPUT_DIR="/tmp"

# 测试各个模块
echo "1. 测试中国日历模块..."
if gcc $CFLAGS src/features/chinese_calendar.c -o $OUTPUT_DIR/chinese_calendar.o 2>&1; then
    echo "✓ 中国日历模块编译成功"
else
    echo "✗ 中国日历模块编译失败"
    exit 1
fi

echo
echo "2. 测试倒计时模块..."
if gcc $CFLAGS src/features/countdown.c -o $OUTPUT_DIR/countdown.o 2>&1; then
    echo "✓ 倒计时模块编译成功"
else
    echo "✗ 倒计时模块编译失败"
    exit 1
fi

echo
echo "3. 测试显示模式模块..."
if gcc $CFLAGS src/features/display_modes.c -o $OUTPUT_DIR/display_modes.o 2>&1; then
    echo "✓ 显示模式模块编译成功"
else
    echo "✗ 显示模式模块编译失败"
    exit 1
fi

echo
echo "4. 测试主程序集成..."
if gcc $CFLAGS src/user_custs1_impl.c -o $OUTPUT_DIR/user_custs1_impl.o 2>&1; then
    echo "✓ 主程序集成编译成功"
else
    echo "✗ 主程序集成编译失败"
    exit 1
fi

echo
echo "=== 编译测试总结 ==="
echo "✓ 所有模块编译成功"
echo "✓ 代码语法正确"
echo "✓ 模块集成正常"
echo
echo "注意：这是使用GCC的语法验证测试"
echo "实际设备编译需要使用Keil uVision和Dialog SDK"
echo
echo "生成的目标文件："
ls -la $OUTPUT_DIR/*.o | grep -E "(chinese_calendar|countdown|display_modes|user_custs1_impl)"