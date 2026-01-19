/**
 * @file enum_demo.cpp
 * @brief 演示 C++ 枚举 (Enum) 的基础用法与特性
 */

#include <iostream>

// ==========================================
// 1. 基本定义 (默认从 0 开始)
// ==========================================
// 应用场景：日志级别
enum LogLevel
{
    LOG_ERROR,   // 0
    LOG_WARNING, // 1
    LOG_INFO     // 2
};

// ==========================================
// 2. 手动指定数值
// ==========================================
// 应用场景：自定义错误码或特定的数值映射
enum CustomValue
{
    START = 10, // 手动指定为 10
    MIDDLE,     // 自动递增 -> 11
    END = 20    // 手动指定为 20
};

// ==========================================
// 3. 指定底层类型 (C++11 特性)
// ==========================================
// 默认 enum 占用 4 字节，指定 unsigned char 后仅占用 1 字节，节省内存
enum SmallState : unsigned char
{
    STATE_IDLE,
    STATE_RUNNING,
    STATE_GAMEOVER
};

int main()
{
    // 使用枚举变量
    LogLevel currentLevel = LOG_WARNING;

    // -----------------------------------
    // 演示 1: 枚举作为整数的本质
    // -----------------------------------
    std::cout << "--- Enum Values ---" << std::endl;
    std::cout << "LOG_ERROR value: " << LOG_ERROR << std::endl;     // 输出 0
    std::cout << "LOG_WARNING value: " << LOG_WARNING << std::endl; // 输出 1
    std::cout << "CustomValue MIDDLE: " << MIDDLE << std::endl;     // 输出 11

    // -----------------------------------
    // 演示 2: 在 Switch 语句中的应用 (最常用)
    // -----------------------------------
    // 模拟一个游戏状态
    SmallState playerState = STATE_RUNNING;

    std::cout << "\n--- Switch Logic ---" << std::endl;
    switch (playerState)
    {
    case STATE_IDLE:
        std::cout << "Player is standing still." << std::endl;
        break;
    case STATE_RUNNING:
        std::cout << "Player is running!" << std::endl;
        break;
    case STATE_GAMEOVER:
        std::cout << "Game Over." << std::endl;
        break;
    default:
        std::cout << "Unknown state." << std::endl;
        break;
    }

    // -----------------------------------
    // 演示 3: 内存占用对比
    // -----------------------------------
    std::cout << "\n--- Memory Size ---" << std::endl;
    std::cout << "Size of LogLevel (Default int): " << sizeof(LogLevel) << " bytes" << std::endl;
    std::cout << "Size of SmallState (unsigned char): " << sizeof(SmallState) << " bytes" << std::endl;

    return 0;
}