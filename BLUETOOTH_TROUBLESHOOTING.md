# 🔧 蓝牙连接问题排查指南

## 问题现象
Web蓝牙搜索不到电子墨水屏设备，即使蓝牙权限已正常授予。

## 🔍 问题排查步骤

### 1. 使用蓝牙调试工具
我们提供了专门的调试工具来帮助您找到设备：

**访问调试工具：** `debug_bluetooth.html`

#### 调试工具功能
- 🔍 **扫描所有设备** - 显示附近所有蓝牙设备
- ⏰ **扫描时钟设备** - 使用多种名称模式搜索
- 🎯 **自定义扫描** - 根据您的设备名称搜索
- 📊 **设备信息显示** - 查看设备详细信息
- 🔗 **连接测试** - 验证设备连接能力

### 2. 检查设备状态

#### 设备是否处于广播状态？
- ✅ 设备已开机且正常运行
- ✅ 设备未被其他应用连接
- ✅ 设备距离在1-2米范围内
- ✅ 设备电量充足

#### 常见设备名称
电子墨水屏设备可能使用以下名称：
- `DLG-CLOCK-XXXX`
- `CLOCK-XXXX` 
- `DLG-XXXX`
- `HM-XXXX`
- `EPD-XXXX`
- 或其他自定义名称

### 3. 浏览器兼容性检查

#### 支持的浏览器
- ✅ **Chrome 56+** (推荐)
- ✅ **Microsoft Edge 79+** (推荐)
- ✅ **Opera 43+**
- ❌ Firefox (不支持Web Bluetooth)
- ❌ Safari (不支持Web Bluetooth)

#### 检查Web Bluetooth支持
```javascript
if ('bluetooth' in navigator) {
    console.log('浏览器支持Web Bluetooth');
} else {
    console.log('浏览器不支持Web Bluetooth');
}
```

### 4. 权限和安全检查

#### HTTPS要求
- ✅ 网站必须使用HTTPS协议
- ✅ 本地测试可使用 `localhost` 或 `127.0.0.1`

#### 蓝牙权限
1. 检查浏览器蓝牙权限设置
2. 确保网站有蓝牙访问权限
3. 尝试重新授权蓝牙权限

#### 检查方法
- Chrome: 地址栏左侧锁图标 → 权限设置
- Edge: 地址栏左侧锁图标 → 权限

### 5. 系统蓝牙检查

#### Windows系统
1. 确保系统蓝牙已开启
2. 检查蓝牙驱动是否正常
3. 尝试重启蓝牙服务

#### macOS系统
1. 系统偏好设置 → 蓝牙 → 确保已开启
2. 检查是否有其他应用占用蓝牙

#### Linux系统
1. 确保 `bluetoothd` 服务运行
2. 检查用户权限组设置

### 6. 设备特定问题

#### 盒马价签设备
- 设备名称通常为 `DLG-CLOCK-XXXX`
- 服务UUID: `0xff00`
- 特征值UUID: `0xff01`, `0xff02`, `0xff03`

#### 检查设备固件
- 确保设备运行的是支持蓝牙的固件版本
- 检查设备是否已正确烧录固件

## 🛠️ 解决方案

### 方案1: 使用调试工具
1. 打开 `debug_bluetooth.html`
2. 点击"扫描所有设备"
3. 查看是否能找到您的设备
4. 记录设备的确切名称

### 方案2: 修改设备过滤条件
如果您知道设备的确切名称，可以修改Web界面的搜索条件：

```javascript
// 在 enhanced_weble.html 中修改
device = await navigator.bluetooth.requestDevice({
    filters: [
        { namePrefix: "您的设备名称前缀" },
        { name: "您的设备完整名称" }
    ],
    optionalServices: [ 0xff00 ]
});
```

### 方案3: 使用acceptAllDevices
如果仍然找不到设备，可以尝试扫描所有设备：

```javascript
device = await navigator.bluetooth.requestDevice({
    acceptAllDevices: true,
    optionalServices: [ 0xff00 ]
});
```

### 方案4: 重置蓝牙连接
1. 关闭浏览器
2. 重启系统蓝牙
3. 重新打开浏览器
4. 清除浏览器缓存
5. 重新尝试连接

### 方案5: 使用原生蓝牙工具验证
使用系统自带的蓝牙工具搜索设备：
- Windows: 设置 → 蓝牙和其他设备
- macOS: 系统偏好设置 → 蓝牙
- Linux: `bluetoothctl scan on`

## 📋 常见错误信息

### "User cancelled the requestDevice() chooser"
- **原因**: 用户取消了设备选择
- **解决**: 重新点击连接按钮

### "Bluetooth adapter not available"
- **原因**: 系统蓝牙未开启或不可用
- **解决**: 检查系统蓝牙设置

### "Origin is not allowed to use the Web Bluetooth API"
- **原因**: 网站不是HTTPS或不在允许列表中
- **解决**: 使用HTTPS访问网站

### "No devices found"
- **原因**: 设备不在范围内或名称不匹配
- **解决**: 检查设备状态和搜索条件

### "GATT Server is disconnected"
- **原因**: 设备连接中断
- **解决**: 重新连接设备

## 🔧 高级调试

### 启用Chrome蓝牙调试
1. 打开 `chrome://bluetooth-internals/`
2. 查看蓝牙适配器状态
3. 监控设备广播信息

### 查看控制台日志
1. 按F12打开开发者工具
2. 查看Console标签页
3. 寻找蓝牙相关错误信息

### 网络抓包分析
使用蓝牙抓包工具分析设备广播数据：
- Windows: Bluetooth LE Explorer
- Android: nRF Connect
- 通用: Wireshark + Bluetooth插件

## 📞 获取帮助

### 自助诊断
1. 使用 `debug_bluetooth.html` 工具
2. 查看浏览器控制台错误信息
3. 检查设备在系统蓝牙中是否可见

### 提供信息
如需技术支持，请提供：
- 浏览器类型和版本
- 操作系统版本
- 设备型号和固件版本
- 错误信息截图
- 调试工具的扫描结果

### 替代方案
如果Web蓝牙无法正常工作，可以考虑：
- 使用移动端APP
- 使用串口连接
- 使用专用的蓝牙调试工具

## ✅ 成功连接检查清单

- [ ] 浏览器支持Web Bluetooth API
- [ ] 网站使用HTTPS协议
- [ ] 系统蓝牙已开启
- [ ] 设备处于广播状态
- [ ] 设备在有效范围内
- [ ] 蓝牙权限已授予
- [ ] 设备名称匹配搜索条件
- [ ] 设备服务UUID正确
- [ ] 没有其他应用占用设备

按照这个指南逐步排查，应该能够解决大部分蓝牙连接问题。