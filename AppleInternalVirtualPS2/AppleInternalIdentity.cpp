#include "AppleInternalIdentity.hpp"

namespace AppleIdentity {
    // 强制使用示例中的白苹果 ID 和版本
    const UInt32 VendorID  = 0x05AC;
    const UInt32 ProductID = 0x0276;
    const UInt32 VersionNumber = 0x0896;
    const UInt32 CountryCode   = 0x21;
//5
    const char* Manufacturer = "Apple Inc.";
    const char* Product      = "Apple Keyboard";
    const char* Transport    = "USB";


const UInt8 ReportDescriptor[] = {
    // Keyboard / Generic Desktop
    0x05, 0x01,       // Usage Page (Generic Desktop)
    0x09, 0x06,       // Usage (Keyboard)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x01,       //   Report ID (1)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0xE0,       //   Usage Minimum (224) - Modifiers
    0x29, 0xE7,       //   Usage Maximum (231) - Modifiers
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x08,       //   Report Count (8)
    0x81, 0x02,       //   Input (Data,Var,Abs) - Modifier byte
    0x95, 0x01,       //   Report Count (1)
    0x75, 0x08,       //   Report Size (8)
    0x81, 0x01,       //   Input (Const,Array,Abs) - Reserved byte
    0x95, 0x05,       //   Report Count (5)
    0x75, 0x01,       //   Report Size (1)
    0x05, 0x08,       //   Usage Page (LEDs)
    0x19, 0x01,       //   Usage Minimum (1) - Num Lock
    0x29, 0x05,       //   Usage Maximum (5) - Kana
    0x91, 0x02,       //   Output (Data,Var,Abs) - LED report
    0x95, 0x01,       //   Report Count (1)
    0x75, 0x03,       //   Report Size (3)
    0x91, 0x01,       //   Output (Const,Array,Abs) - LED padding
    0x95, 0x06,       //   Report Count (6)
    0x75, 0x08,       //   Report Size (8)
    0x15, 0x00,       //   Logical Minimum (0)
    0x26, 0xFF, 0x00, //   Logical Maximum (255)
    0x05, 0x07,       //   Usage Page (Keyboard/Keypad)
    0x19, 0x00,       //   Usage Minimum (0)
    0x29, 0xFF,       //   Usage Maximum (255)
    0x81, 0x00,       //   Input (Data,Array,Abs) - Key codes
    0x05, 0x0C,       //   Usage Page (Consumer)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x01,       //   Report Count (1)
    0x09, 0xB8,       //   Usage (Eject)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x05, 0xFF,       //   Usage Page (Vendor Defined)
    0x09, 0x03,       //   Usage (Vendor Usage 3)
    0x75, 0x07,       //   Report Size (7)
    0x95, 0x01,       //   Report Count (1)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0xC0,             // End Collection

    // Consumer Control (Media Keys)
    0x05, 0x0C,       // Usage Page (Consumer)
    0x09, 0x01,       // Usage (Consumer Control)
    0xA1, 0x01,       // Collection (Application)
    0x85, 0x52,       //   Report ID (82)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x01,       //   Report Size (1)
    0x95, 0x01,       //   Report Count (1)
    0x09, 0xCD,       //   Usage (Play/Pause)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x09, 0xB3,       //   Usage (Fast Forward)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x09, 0xB4,       //   Usage (Rewind)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x09, 0xB5,       //   Usage (Scan Next Track)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x09, 0xB6,       //   Usage (Scan Previous Track)
    0x81, 0x02,       //   Input (Data,Var,Abs)
    0x81, 0x01,       //   Input (Const,Array,Abs) - Padding
    0x81, 0x01,       //   Input (Const,Array,Abs) - Padding
    0x81, 0x01,       //   Input (Const,Array,Abs) - Padding
    
    // Vendor Specific Configuration
    0x85, 0x09,       //   Report ID (9)
    0x15, 0x00,       //   Logical Minimum (0)
    0x25, 0x01,       //   Logical Maximum (1)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x01,       //   Report Count (1)
    0x06, 0x01, 0xFF, //   Usage Page (Vendor Page 1)
    0x09, 0x0B,       //   Usage (Vendor Usage 11)
    0xB1, 0x02,       //   Feature (Data,Var,Abs)
    0x75, 0x08,       //   Report Size (8)
    0x95, 0x02,       //   Report Count (2)
    0xB1, 0x01,       //   Feature (Const,Array,Abs)
    0xC0              // End Collection
};
    const IOByteCount ReportDescriptorSize = sizeof(ReportDescriptor);
}
