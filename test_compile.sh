#!/bin/bash

# 简单的编译测试脚本
# 检查代码语法和依赖关系

echo "=== HMCLOCK 编译测试 ==="

# 检查源文件是否存在
echo "检查源文件..."
files=(
    "src/features/chinese_calendar.h"
    "src/features/chinese_calendar.c"
    "src/features/countdown.h"
    "src/features/countdown.c"
    "src/features/display_modes.h"
    "src/features/display_modes.c"
    "src/user_peripheral.c"
    "src/user_custs1_impl.c"
)

for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file"
    else
        echo "✗ $file 不存在"
    fi
done

# 检查语法错误（简单检查）
echo -e "\n检查语法..."

# 检查头文件包含
echo "检查头文件包含..."
grep -n "#include.*features" src/user_peripheral.c src/user_custs1_impl.c

# 检查函数调用
echo -e "\n检查函数调用..."
grep -n "chinese_calendar_init\|countdown_init\|display_modes_init" src/user_peripheral.c
grep -n "display_render\|get_chinese_date\|countdown_get_next_event" src/user_custs1_impl.c

# 检查Web界面
echo -e "\n检查Web界面..."
if [ -f "weble/weble.html" ]; then
    echo "✓ Web界面文件存在"
    echo "文件大小: $(wc -c < weble/weble.html) 字节"
    echo "行数: $(wc -l < weble/weble.html) 行"
else
    echo "✗ Web界面文件不存在"
fi

echo -e "\n=== 编译测试完成 ==="
echo "注意: 这只是基本的文件和语法检查"
echo "实际编译需要完整的SDK环境"