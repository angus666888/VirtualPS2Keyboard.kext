import SwiftUI

struct ContentView: View {
    @ObservedObject var controller = KeyboardController.shared
    
    var body: some View {
        VStack(spacing: 6) {
            // 第一排：功能键区
            HStack(spacing: 4) {
                KeyCapView("ESC", code: HIDUsage.esc, color: .orange, width: 50)
                Spacer().frame(width: 15)
                ForEach(1...12, id: \.self) { i in
                    KeyCapView("F\(i)", code: HIDUsage.fKey(i), width: 40)
                }
            }
            
            // 第二排：数字与符号
            HStack(spacing: 4) {
                KeyCapView("`", code: 0x35)
                ForEach(1...9, id: \.self) { i in KeyCapView("\(i)", code: HIDUsage.number(i)) }
                KeyCapView("0", code: HIDUsage.number(0))
                KeyCapView("-", code: 0x2D); KeyCapView("=", code: 0x2E)
                KeyCapView("⌫", code: HIDUsage.backspace, color: .red, width: 60)
            }
            
            // 第三排：QWERTY
            HStack(spacing: 4) {
                KeyCapView("Tab", code: HIDUsage.tab, width: 55)
                ForEach(["Q","W","E","R","T","Y","U","I","O","P"], id: \.self) { char in
                    KeyCapView(char, code: HIDUsage.letter(char))
                }
                KeyCapView("[", code: 0x2F); KeyCapView("]", code: 0x30); KeyCapView("\\", code: 0x31)
            }
            
            // 第四排：ASDF
            HStack(spacing: 4) {
                KeyCapView("Caps", code: HIDUsage.capsLock, width: 65)
                ForEach(["A","S","D","F","G","H","J","K","L"], id: \.self) { char in
                    KeyCapView(char, code: HIDUsage.letter(char))
                }
                KeyCapView(";", code: 0x33); KeyCapView("'", code: 0x34)
                KeyCapView("Enter", code: HIDUsage.enter, color: .blue, width: 75)
            }
            
            // 第五排：Shift 组合
            HStack(spacing: 4) {
                ModifierKeyView(label:"Shift", mod: HIDUsage.shift, width: 85)
                ForEach(["Z","X","C","V","B","N","M"], id: \.self) { char in
                    KeyCapView(char, code: HIDUsage.letter(char))
                }
                KeyCapView(",", code: 0x36); KeyCapView(".", code: 0x37); KeyCapView("/", code: 0x38)
                KeyCapView("↑", code: HIDUsage.up)
            }
            
            // 第六排：控制
            HStack(spacing: 4) {
                ModifierKeyView(label:"⌘ Cmd", mod: HIDUsage.gui, width: 75)
                ModifierKeyView(label:"⌥ Alt", mod: HIDUsage.alt, width: 75)
                KeyCapView("Space", code: HIDUsage.space, width: 220)
                KeyCapView("←", code: HIDUsage.left)
                KeyCapView("↓", code: HIDUsage.down)
                KeyCapView("→", code: HIDUsage.right)
            }
        }
        .padding()
        .frame(minWidth: 650)
        .background(Color(NSColor.windowBackgroundColor))
    }
}

// 标准按键视图
struct KeyCapView: View {
    let label: String
    let code: UInt8
    var color: Color = .secondary
    var width: CGFloat = 42

    init(_ label: String, code: UInt8, color: Color = .secondary, width: CGFloat = 42) {
        self.label = label
        self.code = code
        self.color = color
        self.width = width
    }

    var body: some View {
            Text(label) // 使用 Text 代替 Button 以手动控制点击
                .font(.system(size: 11, weight: .bold))
                .frame(width: width, height: 38)
                .background(color.opacity(0.8))
                .foregroundColor(.white)
                .cornerRadius(6)
                // 使用点击手势而非 Button，这样它绝不会抢夺焦点
                .onTapGesture {
                    KeyboardController.shared.postKey(keycode: code)
                }
                // 确保鼠标移上去时不会显示任何焦点边框
                .focusable(false)
        }
}
// 修饰键视图 (具有状态显示)
struct ModifierKeyView: View {
    let label: String
    let mod: UInt8
    let width: CGFloat
    @ObservedObject var controller = KeyboardController.shared
    
    var isActive: Bool { (controller.activeModifiers & mod) != 0 }

    var body: some View {
        Button(action: {
            controller.toggleModifier(mod)
        }) {
            Text(label)
                .font(.system(size: 11, weight: .black))
                .frame(width: width, height: 38)
                .background(isActive ? Color.green : Color.gray)
                .foregroundColor(.white)
                .cornerRadius(6)
        }
        .buttonStyle(PlainButtonStyle())
    }
}
