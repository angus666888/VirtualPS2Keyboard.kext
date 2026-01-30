#include "PS2EventInterceptor.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/IOWorkLoop.h>
#include <architecture/i386/pio.h>

#define PS2_DATA_PORT 0x60

OSDefineMetaClassAndStructors(PS2EventInterceptor, IOService)

bool PS2EventInterceptor::start(IOService* provider) {
    // 强制打印，确认 start 跑到了
    IOLog("ANGUS_LOG: Start called on provider: %s\n", provider->getName());

    if (!IOService::start(provider)) return false;

    IOWorkLoop* wl = getWorkLoop();
    if (!wl) {
        IOLog("ANGUS_LOG: Failed to get WorkLoop\n");
        return false;
    }

    interruptSource = IOInterruptEventSource::interruptEventSource(
        this,
        (IOInterruptEventAction)&PS2EventInterceptor::interruptHandler,
        provider,
        0
    );

    if (!interruptSource) {
        IOLog("ANGUS_LOG: Failed to create InterruptSource\n");
        return false;
    }

    IOReturn res = wl->addEventSource(interruptSource);
    if (res != kIOReturnSuccess) {
        IOLog("ANGUS_LOG: Failed to add EventSource to WorkLoop, error: 0x%x\n", res);
        return false;
    }

    interruptSource->enable();
    IOLog("ANGUS_LOG: IRQ 1 Enable sequence complete\n");

    registerService();
    return true;

}

void PS2EventInterceptor::interruptHandler(OSObject* owner, IOInterruptEventSource* src, int count) {
    // 1. 获取当前类的实例 (虽然现在没用到，但这是调用类内其他方法的标准做法)
    // PS2EventInterceptor* self = OSDynamicCast(PS2EventInterceptor, owner);
    
    // 2. 核心动作：读取 0x60 端口
    uint8_t scancode = inb(PS2_DATA_PORT);
    
    // 3. 打印
    IOLog("ANGUS_LOG: [RAW_PS2] 0x%02X\n", scancode);
}

void PS2EventInterceptor::stop(IOService *provider) {
    if (interruptSource) {
        interruptSource->disable();
        if (getWorkLoop()) {
            getWorkLoop()->removeEventSource(interruptSource);
        }
    }
    IOLog("ANGUS_LOG: [INTERCEPTOR] Stopped.\n");
    IOService::stop(provider);
}

void PS2EventInterceptor::free() {
    // 彻底清理内存
    OSSafeReleaseNULL(interruptSource);
    IOService::free();
}
