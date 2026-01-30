import Foundation

struct HIDUsage {
    // 修饰键掩码
    static let ctrl:  UInt8 = 0x01
    static let shift: UInt8 = 0x02
    static let alt:   UInt8 = 0x04
    static let gui:   UInt8 = 0x08 // Command

    // 常用功能键
    static let esc:       UInt8 = 0x29
    static let enter:     UInt8 = 0x28
    static let backspace: UInt8 = 0x2A
    static let tab:       UInt8 = 0x2B
    static let space:     UInt8 = 0x2C
    static let capsLock:  UInt8 = 0x39

    // 方向键
    static let up:    UInt8 = 0x52
    static let down:  UInt8 = 0x51
    static let left:  UInt8 = 0x50
    static let right: UInt8 = 0x4F

    // F 键
    static func fKey(_ n: Int) -> UInt8 { return UInt8(0x39 + n) }

    // 字母映射
    static func letter(_ char: String) -> UInt8 {
        let mapping: [String: UInt8] = [
            "A": 0x04, "B": 0x05, "C": 0x06, "D": 0x07, "E": 0x08, "F": 0x09, "G": 0x0A,
            "H": 0x0B, "I": 0x0C, "J": 0x0D, "K": 0x0E, "L": 0x0F, "M": 0x10, "N": 0x11,
            "O": 0x12, "P": 0x13, "Q": 0x14, "R": 0x15, "S": 0x16, "T": 0x17, "U": 0x18,
            "V": 0x19, "W": 0x1A, "X": 0x1B, "Y": 0x1C, "Z": 0x1D
        ]
        return mapping[char.uppercased()] ?? 0x00
    }

    // 数字映射
    static func number(_ n: Int) -> UInt8 {
        if n == 0 { return 0x27 }
        return UInt8(0x1D + n)
    }
    
    // 符号映射
    static let symbolMapping: [String: UInt8] = [
        "-": 0x2D, "=": 0x2E, "[": 0x2F, "]": 0x30, "\\": 0x31,
        ";": 0x33, "'": 0x34, "`": 0x35, ",": 0x36, ".": 0x37, "/": 0x38
    ]
}
