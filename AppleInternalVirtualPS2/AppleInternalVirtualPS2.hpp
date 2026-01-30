#ifndef AppleInternalVirtualPS2_hpp
#define AppleInternalVirtualPS2_hpp

#include <IOKit/hid/IOHIDDevice.h>

class AppleInternalVirtualPS2 : public IOHIDDevice {
    OSDeclareDefaultStructors(AppleInternalVirtualPS2)

public:
    // --- 生命周期 ---
    virtual bool init(OSDictionary* dict = nullptr) override;
    virtual bool handleStart(IOService* provider) override;

    virtual IOReturn newUserClient(task_t owningTask, void * securityID,
                                       UInt32 type, OSDictionary * properties,
                                       IOUserClient ** handler) override;
    virtual IOReturn newReportDescriptor(IOMemoryDescriptor **descriptor) const override;
    virtual IOReturn getReport(IOMemoryDescriptor *report, IOHIDReportType reportType, IOOptionBits options) override;

    // --- 身份信息重写 (虚函数双保险) ---
    virtual OSString* newManufacturerString() const override;
    virtual OSString* newProductString() const override;
    virtual OSNumber* newVendorIDNumber() const override;
    virtual OSNumber* newProductIDNumber() const override;
    virtual OSNumber* newLocationIDNumber() const override;
    virtual OSNumber* newCountryCodeNumber() const override;
    virtual OSNumber* newVersionNumber() const override;
    virtual OSNumber* newPrimaryUsagePageNumber() const override;
    virtual OSNumber* newPrimaryUsageNumber() const override;

    // --- 数据发送接口 ---
    void postKeyboardReport(UInt8 modifiers, UInt8 key);
};

#endif /* AppleInternalVirtualPS2_hpp */
