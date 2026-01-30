#ifndef AppleInternalUserClient_hpp
#define AppleInternalUserClient_hpp

#include <IOKit/IOUserClient.h>
#include "AppleInternalVirtualPS2.hpp"
#include "SharedTypes.hpp"

class AppleInternalUserClient : public IOUserClient {
    OSDeclareDefaultStructors(AppleInternalUserClient)

private:
    AppleInternalVirtualPS2* fProvider;

    // --- 核心声明：在这里添加 sMethods ---
    // static 表示它属于这个类，const 表示它是只读的查找表
    static const IOExternalMethodDispatch sMethods[kMethodCount];
public:
    // 生命周期
    virtual bool start(IOService* provider) override;
    virtual IOReturn clientClose() override;
    
    // 外部方法分发
    virtual IOReturn externalMethod(uint32_t selector,
                                   IOExternalMethodArguments *arguments,
                                   IOExternalMethodDispatch *dispatch = NULL,
                                   OSObject *target = NULL,
                                   void *reference = NULL) override;

    // 必须是 static 且签名严格匹配 IOExternalMethodAction
    static IOReturn methodPostKey(OSObject* target, void* reference, IOExternalMethodArguments* arguments);
        
        // 模式 1：获取状态逻辑（之前报错就是因为这里漏了）
        static IOReturn methodGetStatus(OSObject* target, void* reference, IOExternalMethodArguments* arguments);
};

#endif
