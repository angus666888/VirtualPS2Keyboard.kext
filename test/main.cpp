#include <iostream>
#include <IOKit/IOKitLib.h>
#include "SharedTypes.hpp"

int main() {
    // --- 配置区 ---
    int mode = 0;            // 1: 打印模拟输出, 0: 真正给驱动, 2: 获取状态
    AppleInternalKeyData data;
    data.modifiers = 0;
    data.keycode = 0x04;     // 'A'
    
    if (mode == 1) {
        // --- 满足你要求的 Struct 输出功能 ---
        std::cout << "--- [模拟模式] 准备上报的内容 ---" << std::endl;
        std::cout << "频道 (Selector): " << kMethodPostScanCode << std::endl;
        std::cout << "结构体大小: " << sizeof(data) << " 字节" << std::endl;
        std::cout << "Modifiers: " << (int)data.modifiers << std::endl;
        std::cout << "KeyCode:   " << (int)data.keycode << " (0x" << std::hex << (int)data.keycode << ")" << std::endl;
        std::cout << "-------------------------------" << std::endl;
        return 0;
    }

    // --- 真实驱动通信逻辑 ---
    io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("AppleInternalVirtualPS2"));
    if (!service) return -1;

    io_connect_t connection;
    if (IOServiceOpen(service, mach_task_self(), 0, &connection) != kIOReturnSuccess) return -1;
    IOObjectRelease(service);

    if (mode == 2) {
        uint64_t status = 0;
        uint32_t count = 1;
        IOConnectCallScalarMethod(connection, kMethodGetStatus, NULL, 0, &status, &count);
        std::cout << "驱动反馈状态: " << status << std::endl;
    } else {
        // 模式 0: 给驱动
        uint64_t input[2] = { data.modifiers, data.keycode };
        kern_return_t kr = IOConnectCallScalarMethod(connection, kMethodPostScanCode, input, 2, NULL, NULL);
        if (kr == kIOReturnSuccess) std::cout << "成功送达内核" << std::endl;
    }

    IOServiceClose(connection);
    return 0;
}
