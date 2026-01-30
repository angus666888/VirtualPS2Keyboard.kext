#ifndef AppleInternalIdentity_hpp
#define AppleInternalIdentity_hpp

#include <libkern/OSTypes.h>
#include <IOKit/IOTypes.h>

namespace AppleIdentity {
    extern const UInt32 VendorID;
    extern const UInt32 ProductID;
    extern const UInt32 VersionNumber;
    extern const UInt32 CountryCode;

    extern const char* Manufacturer;
    extern const char* Product;
    extern const char* Transport;

    extern const UInt8 ReportDescriptor[];
    extern const IOByteCount ReportDescriptorSize;
}

#endif /* AppleInternalIdentity_hpp */
