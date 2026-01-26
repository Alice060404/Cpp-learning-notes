#include <iostream>

               // 定义日志等级 (Log Level)
               // 使用枚举类型提高代码可读性
               enum class LogLevel {
                   LevelError = 0, // 仅错误
                   LevelWarning,   // 错误 + 警告
                   LevelInfo       // 所有信息
               };

class Log
{
private:
    LogLevel m_LogLevel = LogLevel::LevelInfo; // 默认显示所有信息

public:
    // 设置日志等级
    void SetLevel(LogLevel level)
    {
        m_LogLevel = level;
    }

    // 打印错误 (Error) - 最严重
    void Error(const char *message)
    {
        if (m_LogLevel >= LogLevel::LevelError)
        {
            std::cout << "[ERROR]: " << message << std::endl;
        }
    }

    // 打印警告 (Warning)
    void Warn(const char *message)
    {
        if (m_LogLevel >= LogLevel::LevelWarning)
        {
            std::cout << "[WARNING]: " << message << std::endl;
        }
    }

    // 打印信息 (Info) - 普通
    void Info(const char *message)
    {
        if (m_LogLevel >= LogLevel::LevelInfo)
        {
            std::cout << "[INFO]: " << message << std::endl;
        }
    }
};

int main()
{
    Log log;

    std::cout << "=== 测试 1: 默认等级 (Info) ===" << std::endl;
    log.Info("程序开始运行...");   // 会打印
    log.Warn("检测到轻微卡顿");    // 会打印
    log.Error("连接服务器失败！"); // 会打印

    std::cout << "\n=== 测试 2: 调整为 Warning 等级 ===" << std::endl;
    log.SetLevel(LogLevel::LevelWarning);
    log.Info("这条普通消息会被忽略");    // 不会打印
    log.Warn("再次警告：CPU温度过高");   // 会打印
    log.Error("严重错误：核心文件丢失"); // 会打印

    std::cout << "\n=== 测试 3: 调整为 Error 等级 ===" << std::endl;
    log.SetLevel(LogLevel::LevelError);
    log.Info("不显示");    // 不会打印
    log.Warn("不显示");    // 不会打印
    log.Error("系统崩溃"); // 会打印

    return 0;
}