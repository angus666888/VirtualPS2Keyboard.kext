import SwiftUI
import AppKit

@main
struct testuiApp: App {
    // 使用 AppDelegate 接管窗口生命周期
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate

    var body: some Scene {
        // 这里的 Settings 只是占位，不创建默认窗口
        Settings { }
    }
}

// 1. 创建自定义 HostingView，解决“第一下点击没反应”的问题
class FirstMouseHostingView<Content: View>: NSHostingView<Content> {
    // 在视图层级重写此方法，允许第一下点击直接穿透给内部按钮
    override func acceptsFirstMouse(for event: NSEvent?) -> Bool {
        return true
    }
}

// 2. 自定义 Panel 类，确保不抢占焦点
class FloatingPanel: NSPanel {
    // 永远不成为 Key Window (不拿走光标)
    override var canBecomeKey: Bool { return false }
    // 永远不成为 Main Window
    override var canBecomeMain: Bool { return false }
}

// 3. 核心启动控制器
class AppDelegate: NSObject, NSApplicationDelegate {
    var panel: FloatingPanel!

    func applicationDidFinishLaunching(_ notification: Notification) {
        // 创建面板
        panel = FloatingPanel(
            contentRect: NSRect(x: 100, y: 100, width: 800, height: 400),
            styleMask: [
                .nonactivatingPanel, // 核心：不激活 App
                .titled,
                .resizable,
                .closable,
                .fullSizeContentView,
                .hudWindow // 可选：深色毛玻璃外观
            ],
            backing: .buffered,
            defer: false
        )
        
        // 置顶设置
        panel.level = .floating
        panel.isFloatingPanel = true
        
        // 即使在全屏 App 之上也能显示
        panel.collectionBehavior = [.canJoinAllSpaces, .fullScreenAuxiliary]
        
        // 点击时不强制将 App 调到前台
        panel.becomesKeyOnlyIfNeeded = true
        
        // 使用自定义的 FirstMouseHostingView 包装 ContentView
        // 这样点击键盘任何地方，都不会因为“寻找焦点”而导致第一下点击失效
        let contentView = ContentView()
        panel.contentView = FirstMouseHostingView(rootView: contentView)
        
        // 显示窗口
        panel.makeKeyAndOrderFront(nil)
        panel.orderFrontRegardless()
    }
}
