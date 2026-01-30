import Foundation
import IOKit
import Combine

class KeyboardController: ObservableObject {
    static let shared = KeyboardController()
    private var connection: io_connect_t = 0
    
    // 追踪修饰键状态。使用 @Published 确保 UI 颜色能实时刷新
    @Published var activeModifiers: UInt8 = 0

    init() {
        // 适配新版 macOS 的端口常量
        let masterPort: mach_port_t
        if #available(macOS 12.0, *) {
            masterPort = kIOMainPortDefault
        } else {
            masterPort = kIOMasterPortDefault
        }

        let service = IOServiceGetMatchingService(masterPort, IOServiceMatching("AppleInternalVirtualPS2"))
        guard service != 0 else {
            print("❌ 驱动未加载。请执行: sudo kextload YourDriver.kext")
            return
        }
        
        // 建立连接
        let kr = IOServiceOpen(service, mach_task_self_, 0, &connection)
        IOObjectRelease(service)
        
        if kr != kIOReturnSuccess {
            print("❌ 建立 UserClient 失败，错误码: \(kr)")
        } else {
            print("✅ 虚拟键盘已成功连接内核驱动")
        }
    }
    
    // 切换修饰键状态。主线程更新确保 UI 安全
    func toggleModifier(_ mod: UInt8) {
        DispatchQueue.main.async {
            if (self.activeModifiers & mod) != 0 {
                self.activeModifiers &= ~mod
            } else {
                self.activeModifiers |= mod
            }
        }
    }

    // 发送按键
    func postKey(keycode: UInt8) {
        guard connection != 0 else {
            print("⚠️ 驱动连接未建立，无法发送按键")
            return
        }
        
        // 这里的 input 必须是 [UInt64]，对应驱动 scalarInput
        var input: [UInt64] = [UInt64(activeModifiers), UInt64(keycode)]
        
        // 调用内核方法
        let kr = IOConnectCallScalarMethod(
            connection,
            UInt32(kMethodPostScanCode), // 确保桥接头文件已包含 SharedTypes.h
            &input, 2,                   // 输入 2 个参数 (mod, key)
            nil, nil                     // 无输出参数
        )
        
        if kr != kIOReturnSuccess {
            print("❌ 内核通信失败，IOReturn: \(kr)")
        }
        
        // 自动释放逻辑：如果不是为了组合键操作，发送后重置
        if activeModifiers == HIDUsage.shift {
            DispatchQueue.main.async {
                self.activeModifiers = 0
            }
        }
    }
}
