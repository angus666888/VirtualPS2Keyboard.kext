#include "AppleInternalUserClient.hpp"
#include <IOKit/IOLib.h>

#define super IOUserClient
OSDefineMetaClassAndStructors(AppleInternalUserClient, IOUserClient)

// 修正：分发表定义
const IOExternalMethodDispatch AppleInternalUserClient::sMethods[kMethodCount] = {
    [kMethodPostScanCode] = {
        (IOExternalMethodAction)&AppleInternalUserClient::methodPostKey,
        2, 0, 0, 0  // 接收 2 个参数 (modifiers, keycode)
    },
    [kMethodGetStatus] = {
        (IOExternalMethodAction)&AppleInternalUserClient::methodGetStatus,
        0, 0, 1, 0  // 预留：接收 0 个，输出 1 个状态值
    }
};

bool AppleInternalUserClient::start(IOService* provider) {
    if (!super::start(provider)) return false;
    
    // 这里的 provider 就是你的 AppleInternalVirtualPS2 实例
    fProvider = OSDynamicCast(AppleInternalVirtualPS2, provider);
    
    return (fProvider != nullptr);
}

IOReturn AppleInternalUserClient::clientClose() {
    // 释放资源，断开连接
    fProvider = nullptr;
    if (!terminate()) {
        IOLog("AppleInternalUserClient: terminate failed.\n");
    }
    return kIOReturnSuccess;
}

IOReturn AppleInternalUserClient::externalMethod(uint32_t selector,
                                               IOExternalMethodArguments* arguments,
                                               IOExternalMethodDispatch* dispatch,
                                               OSObject* target,
                                               void* reference) {
    // 严格边界检查：如果不在枚举范围内，直接屏蔽
    if (selector >= kMethodCount) {
        return kIOReturnUnsupported;
    }

    // 绑定分发表
    dispatch = (IOExternalMethodDispatch*)&sMethods[selector];
    if (!target) target = this;

    return super::externalMethod(selector, arguments, dispatch, target, reference);
}

// 修正：静态方法实现，手动转换 target 指针
IOReturn AppleInternalUserClient::methodPostKey(OSObject* target, void* reference, IOExternalMethodArguments* arguments) {
    AppleInternalUserClient* me = OSDynamicCast(AppleInternalUserClient, target);
    if (!me || !me->fProvider) return kIOReturnNotAttached;

    // 提取 App 传来的 HID 码
    uint8_t mod = (uint8_t)arguments->scalarInput[0];
    uint8_t key = (uint8_t)arguments->scalarInput[1];

    // 直接调用驱动主体的操作逻辑
    me->fProvider->postKeyboardReport(mod, key);
    return kIOReturnSuccess;
}
IOReturn AppleInternalUserClient::methodGetStatus(OSObject* target, void* reference, IOExternalMethodArguments* arguments) {
    arguments->scalarOutput[0] = 1; // 假设返回 1 代表驱动正常
    return kIOReturnSuccess;
}
