# ATK 鼠标电量工具

读取 VGN/ATK 无线鼠标接收器（VID `0x3554`，PID `0xf503`）下鼠标的电量。

## 目录结构

```
atk/
├── src/                      源代码
│   ├── atk-battery.c         控制台版：输出纯数字电量，供 AHK 等脚本调用
│   └── atk-battery-gui.c     托盘版：图标直接显示电量数字，低电量弹窗提醒
├── third_party/hidapi/       第三方库 hidapi（Windows 后端最小子集 + 完整许可文本）
│   ├── include/hidapi.h      公共头文件
│   ├── src/                  库实现及其私有头文件（未做任何修改）
│   └── LICENSE*.txt          上游许可文本，随库保留
├── bin/                      编译产物
├── Makefile
└── readme.md
```

## 编译

需要 MinGW-w64 的 gcc。在项目根目录直接：

```
make
```

或手动执行等价命令：

```
gcc src/atk-battery.c third_party/hidapi/src/hid.c -o bin/atk-battery.exe -I third_party/hidapi/include -static -lsetupapi
gcc src/atk-battery-gui.c third_party/hidapi/src/hid.c -o bin/atk-battery-gui.exe -I third_party/hidapi/include -static -lsetupapi -lgdi32 -mwindows -fexec-charset=GBK
```

说明：`-fexec-charset=GBK` 用于解决托盘气泡中文乱码；hidapi 的 Windows 实现需要链接 `-lsetupapi`。

## 使用

- `bin/atk-battery.exe` — 输出纯数字电量（如 `40`），失败输出 `ERROR`，方便 AHK 等脚本直接取值。
- `bin/atk-battery-gui.exe` — 常驻托盘，每 5 分钟刷新一次图标，电量低于 30% 时弹气泡提醒，右键托盘图标可退出。

注意：查询前请关闭网页驱动（hub.atk.pro），避免网页独占 HID 通道导致读取失败。

## 第三方代码与许可（引用声明）

本项目使用开源库 [hidapi](https://github.com/libusb/hidapi)（libusb/hidapi Team，原作者 Alan Ott / Signal 11 Software）的 Windows 后端，仅保留编译所需的最小文件集，**未修改库源码**。

hidapi 允许使用者从以下三种许可中任选其一（见 `third_party/hidapi/LICENSE.txt`）：

1. GNU General Public License v3.0（`LICENSE-gpl3.txt`）
2. BSD-Style License，即 BSD-3-Clause（`LICENSE-bsd.txt`）
3. 原始 HIDAPI 许可（`LICENSE-orig.txt`）

**本项目选用 BSD-3-Clause 条款使用该库。** 依其要求，若分发本项目的二进制程序或源码，请随附保留上述版权与许可声明（保留 `third_party/hidapi/` 目录即可满足）。
