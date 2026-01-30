#ifndef SharedTypes_h
#define SharedTypes_h
#if defined(__kext__) || defined(KERNEL)
    // 如果是在编写内核驱动，包含 IOKit 类型
    #include <IOKit/IOTypes.h>
    // 在内核中，uint8_t 通常已经定义，或者直接使用 UInt8
#else
    // 如果是在编写用户态 Swift App，包含标准 C 类型库
    #include <stdint.h>
#endif
enum {
    kMethodPostScanCode = 0, // 模式 0：上传扫描码
    kMethodGetStatus    = 1, // 模式 1：获取键盘状态 (预留)
    kMethodCount             // 自动等于 2，用于边界检查
};
// 增加这个结构体，方便 CLI 打印和未来扩展
typedef struct {
    uint8_t  modifiers;
    uint8_t  keycode;
} AppleInternalKeyData;
#endif
