/**
 * @file literals_demo.cpp
 * @brief 深入演示 C++ 字符串字面量：内存差异、原始字符串与后缀
 * @note 兼容 C++14 及以上标准 (因为使用了 "s" 后缀)
 */

#include <iostream>
#include <string>
#include <typeinfo>

// 开启 C++14 字面量后缀支持 (如 "hello"s)
using namespace std::string_literals;

int main()
{
    // ==========================================
    // 1. 内存模型：指针 vs 数组
    // ==========================================
    std::cout << "=== 1. Memory Model: Pointer vs Array ===" << std::endl;

    // [Case A] 指针指向只读常量区
    const char *ptrStr = "Read Only";
    // ptrStr[0] = 'X'; // ? 严重错误！这会导致运行时崩溃 (Segmentation Fault)
    // 解释：操作系统保护了这块内存，禁止写入。

    // [Case B] 数组拷贝到栈上
    char stackArr[] = "Read Only";
    // 注意：这里编译器做了隐式拷贝，将常量区的字符串复制到了栈数组中
    stackArr[0] = 'X'; // ? 安全：修改的是栈上的副本
    std::cout << "Modified Stack Array: " << stackArr << std::endl;

    // ==========================================
    // 2. 原始字符串 (Raw String Literals) - C++11
    // ==========================================
    std::cout << "\n=== 2. Raw String Literals (R\"(...)\") ===" << std::endl;

    // 痛点：普通字符串处理 Windows 路径需要大量转义
    std::string normalPath = "C:\\Program Files\\My App\\config.json";

    // 优势：Raw String 原样输出，所见即所得
    // 语法：R"( 内容 )"
    std::string rawPath = R"(C:\Program Files\My App\config.json)";

    std::cout << "Normal: " << normalPath << std::endl;
    std::cout << "Raw:    " << rawPath << std::endl;

    // Raw String 也非常适合多行文本 (如 ASCII Art 或 JSON)
    std::string json = R"(
    {
        "id": 1,
        "name": "C++",
        "isHard": true
    }
    )";
    std::cout << "Multiline JSON: " << json << std::endl;

    // ==========================================
    // 3. 自动拼接与 UTF-8 前缀
    // ==========================================
    std::cout << "\n=== 3. Concatenation & Prefixes ===" << std::endl;

    // 编译器会自动合并相邻的字面量
    const char *longText = "This is a very long text "
                           "split across multiple lines "
                           "in the source code.";
    std::cout << longText << std::endl;

    // u8 前缀：保证在任何系统上都以 UTF-8 编码存储
    // const char* utf8Str = u8"你好"; // C++11 写法
    // std::cout << "UTF-8 String size: " << sizeof(u8"你好") << " bytes (inc null)" << std::endl;

    // ==========================================
    // 4. 标准库后缀 "s" (C++14)
    // ==========================================
    std::cout << "\n=== 4. C++14 's' Suffix ===" << std::endl;

    // 传统写法：类型是 const char*
    auto str1 = "Hello";
    std::cout << "Type of str1: const char*" << std::endl;

    // C++14 写法：类型直接是 std::string
    // 注意末尾的 's'
    auto str2 = "Hello"s;
    std::cout << "Type of str2: std::string (Size: " << str2.size() << ")" << std::endl;

    // 实际便利：直接进行对象拼接，无需转换
    std::string result = "User: "s + "Gemini"; // 如果没有 s，两个 const char* 不能直接相加
    std::cout << "Concatenated result: " << result << std::endl;

    return 0;
}