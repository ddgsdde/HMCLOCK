#!/bin/bash

echo "=== 增强版电子墨水屏时钟固件编译检查 ==="
echo

# 检查主要源文件是否存在
echo "检查源文件..."
files=(
    "src/user_custs1_impl.c"
    "src/user_peripheral.c" 
    "src/epd/epd_gui.c"
    "src/epd/epd.c"
    "src/epd/epd_hw.c"
    "src/epd/epd.h"
    "Keil_5/ble_app_peripheral.uvprojx"
)

for file in "${files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file"
    else
        echo "✗ $file (缺失)"
    fi
done

echo
echo "检查新增功能文件..."
new_files=(
    "weble/enhanced_weble.html"
    "ENHANCED_FEATURES.md"
    "syntax_check.c"
)

for file in "${new_files[@]}"; do
    if [ -f "$file" ]; then
        echo "✓ $file"
    else
        echo "✗ $file (缺失)"
    fi
done

echo
echo "检查代码语法..."
if gcc -o syntax_check syntax_check.c 2>/dev/null; then
    echo "✓ 语法检查通过"
    ./syntax_check
    rm -f syntax_check
else
    echo "✗ 语法检查失败"
fi

echo
echo "统计代码行数..."
echo "主要源文件:"
wc -l src/user_custs1_impl.c src/epd/epd_gui.c src/epd/epd.h 2>/dev/null | tail -1

echo
echo "新增功能:"
wc -l weble/enhanced_weble.html ENHANCED_FEATURES.md 2>/dev/null | tail -1

echo
echo "=== 编译检查完成 ==="
echo
echo "注意事项:"
echo "1. 需要 Keil 5 + DA14585 SDK 进行实际编译"
echo "2. 使用 Keil_5/ble_app_peripheral.uvprojx 项目文件"
echo "3. 编译前确保 SDK 路径配置正确"
echo "4. 生成的 .bin 文件可直接烧录到设备"
echo
echo "新增功能:"
echo "- 多种显示模式 (时钟/日历/图片/自定义)"
echo "- 时区支持 (UTC-12 到 UTC+12)"
echo "- 反色显示功能"
echo "- 自定义文本与变量替换"
echo "- 扩展的绘图函数"
echo "- 增强版 Web 蓝牙控制界面"