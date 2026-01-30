#include "AppleInternalVirtualPS2.hpp"
#include "AppleInternalIdentity.hpp"
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IOLib.h>
#include "AppleInternalUserClient.hpp"
#define super IOHIDDevice
OSDefineMetaClassAndStructors(AppleInternalVirtualPS2, IOHIDDevice)

bool AppleInternalVirtualPS2::init(OSDictionary* dict) {
    if (!super::init(dict)) return false;
    static int instanceCount = 0;
    if (instanceCount > 0) return false;
    instanceCount++;
    return true;
}


bool AppleInternalVirtualPS2::handleStart(IOService *provider) {
    if (!super::handleStart(provider)) {
        return false;
    }

    setProperty("AppleVendorSupported", kOSBooleanTrue);
    setProperty("Built-In", kOSBooleanTrue);
    setProperty("HIDDefaultBehavior", kOSBooleanTrue);

    registerService();
    return true;
}


IOReturn AppleInternalVirtualPS2::getReport(IOMemoryDescriptor *report, IOHIDReportType reportType, IOOptionBits options) {
    uint8_t report_id = options & 0xff;
    
    if (report_id == 0xbf) {
        // 关键的 8 字节初始化数据
        uint8_t buffer[] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x02, 0x00};
        report->writeBytes(0, buffer, sizeof(buffer));
        return kIOReturnSuccess;
    }

    return super::getReport(report, reportType, options);
}
// 在 AppleInternalVirtualPS2.cpp 中添加
IOReturn AppleInternalVirtualPS2::newUserClient(task_t owningTask, void * securityID,
                                               UInt32 type, OSDictionary * properties,
                                               IOUserClient ** handler) {
    IOReturn res = kIOReturnSuccess;
    
    // 实例化你刚才写的 UserClient 类
    AppleInternalUserClient * client = new AppleInternalUserClient;

    if (!client) return kIOReturnNoMemory;

    // 初始化并关联当前驱动实例 (this)
    if (!client->init() || !client->attach(this) || !client->start(this)) {
        client->release();
        return kIOReturnError;
    }

    // 将创建好的 Client 指针交给系统管理
    *handler = client;
    return kIOReturnSuccess;
}
// 虚函数重写 (对齐示例)
IOReturn AppleInternalVirtualPS2::newReportDescriptor(IOMemoryDescriptor **descriptor) const {
    *descriptor = IOBufferMemoryDescriptor::withBytes(AppleIdentity::ReportDescriptor, AppleIdentity::ReportDescriptorSize, kIODirectionNone);
    return kIOReturnSuccess;
}

OSString *AppleInternalVirtualPS2::newManufacturerString() const {
    return OSString::withCString(AppleIdentity::Manufacturer);
}

OSString *AppleInternalVirtualPS2::newProductString() const {
    return OSString::withCString(AppleIdentity::Product);
}

OSNumber *AppleInternalVirtualPS2::newVendorIDNumber() const {
    return OSNumber::withNumber(AppleIdentity::VendorID, 32);
}

OSNumber *AppleInternalVirtualPS2::newProductIDNumber() const {
    return OSNumber::withNumber(AppleIdentity::ProductID, 32);
}

OSNumber *AppleInternalVirtualPS2::newLocationIDNumber() const {
    return OSNumber::withNumber(0x1000000, 32);
}

OSNumber *AppleInternalVirtualPS2::newCountryCodeNumber() const {
    return OSNumber::withNumber(AppleIdentity::CountryCode, 32);
}

OSNumber *AppleInternalVirtualPS2::newVersionNumber() const {
    return OSNumber::withNumber(AppleIdentity::VersionNumber, 32);
}

OSNumber *AppleInternalVirtualPS2::newPrimaryUsagePageNumber() const {
    return OSNumber::withNumber(0x01, 32); // Generic Desktop
}

OSNumber *AppleInternalVirtualPS2::newPrimaryUsageNumber() const {
    return OSNumber::withNumber(0x06, 32); // Keyboard
}

// 适配新描述符的数据发送
void AppleInternalVirtualPS2::postKeyboardReport(UInt8 modifiers, UInt8 key) {
    // 必须是 10 字节才能对齐描述符
    // [0] ID=1
    // [1] Modifiers
    // [2] Reserved (0x00)
    // [3-8] 6个键位槽 (我们用第一个 report[3])
    // [9] 混合位 (Eject 1bit + Vendor 7bit)
    UInt8 report[10] = { 0x01, modifiers, 0x00, key, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    IOBufferMemoryDescriptor* mem = IOBufferMemoryDescriptor::withBytes(report, sizeof(report), kIODirectionIn);
    
    if (mem) {
        // 执行发送
        handleReport(mem, kIOHIDReportTypeInput);
        
        // 立即发送松开包 (除了 ID 全 0)
        UInt8 release[10] = { 0x01, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        mem->writeBytes(0, release, sizeof(release));
        handleReport(mem, kIOHIDReportTypeInput);
        
        mem->release();
        IOLog("AppleInternal: Sent 10-byte report for key 0x%02X\n", key);
    }
}
