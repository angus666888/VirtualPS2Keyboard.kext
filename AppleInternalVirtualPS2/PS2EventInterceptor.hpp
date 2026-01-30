#ifndef PS2EventInterceptor_hpp
#define PS2EventInterceptor_hpp

#include <IOKit/IOService.h>
#include <IOKit/IOInterruptEventSource.h>

class PS2EventInterceptor : public IOService {
    OSDeclareDefaultStructors(PS2EventInterceptor)

private:
    IOInterruptEventSource* interruptSource;
    
    // 静态回调：内核调用时会把类实例作为第一个参数 (owner) 传进来
    static void interruptHandler(OSObject* owner, IOInterruptEventSource* src, int count);

public:
    virtual bool start(IOService* provider) override;
    virtual void stop(IOService* provider) override;
    virtual void free() override; // 释放内存的最后防线
};

#endif
