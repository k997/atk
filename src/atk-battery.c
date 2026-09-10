#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

int get_battery() {
    struct hid_device_info *devs, *cur_dev;
    hid_device *handle;
    int battery_level = -1; // 用 -1 代表未找到或出错 (对应 Python 中的 None)

    // 初始化 hidapi 库
    if (hid_init() != 0) {
        return -1;
    }

    // 枚举指定 vendor_id 和 product_id 的设备
    devs = hid_enumerate(0x3554, 0xf503);
    cur_dev = devs;

    while (cur_dev) {
        // 尝试通过 path 打开设备
        handle = hid_open_path(cur_dev->path);
        if (handle) {
            // 准备发送的查询指令（17个字节）
            // 第一个字节 8 通常是 Report ID
            unsigned char write_buf[17] = {8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 73};
            
            // 准备接收缓冲
            unsigned char read_buf[16] = {0};

            // 发送查询指令
            int res = hid_write(handle, write_buf, sizeof(write_buf));
            if (res >= 0) {
                // 读取响应数据，最大读取 16 字节，超时设置为 100 毫秒
                res = hid_read_timeout(handle, read_buf, 16, 100);
                
                // 验证响应数据
                if (res >= 7 && read_buf[0] == 8 && read_buf[1] == 4) {
                    battery_level = read_buf[6];
                    hid_close(handle);
                    break; // 成功获取，跳出循环
                }
            }
            // 关闭当前句柄，继续尝试下一个接口
            hid_close(handle);
        }
        cur_dev = cur_dev->next;
    }

    // 释放枚举链表内存
    hid_free_enumeration(devs);
    
    // 退出 hidapi
    hid_exit();

    return battery_level;
}

int main() {
    int val = get_battery();
    
    if (val != -1) {
        // 纯数字输出给 AHK，不换行
        printf("%d", val);
    } else {
        printf("ERROR");
    }
    
    return 0;
}