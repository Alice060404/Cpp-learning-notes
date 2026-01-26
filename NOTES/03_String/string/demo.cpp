/**
 * @file Strings_Note_Fixed.cpp
 * @brief C++ 字符串全解 (兼容 C++98/11/14 版本)
 * @note 即使是老旧的编译器也能直接运行此代码
 */

#include <iostream>
#include <string>
#include <cstring> // 包含 C 风格字符串函数 (strlen, strcpy)

// ==========================================
// 1. C 风格字符串演示 (底层视角)
// ==========================================
void c_style_demo()
{
    std::cout << "--- C-Style Strings ---" << std::endl;

    // 栈上的字符数组 (可修改)
    char mutableStr[] = "Hello";
    mutableStr[0] = 'h'; // ? OK

    // 指向常量区的指针 (不可修改)
    const char *immutableStr = "World";
    // immutableStr[0] = 'w'; // ? 运行时错误：写入只读内存

    std::cout << "Mutable: " << mutableStr << std::endl;

    // 演示空终止符的重要性
    // 手动创建一个没有 \0 的字符数组是非常危险的
    char noNull[] = {'A', 'B', 'C'};
    // std::cout << noNull << std::endl; // ?? 危险！可能会乱码
}

// ==========================================
// 2. std::string 常用功能与 SSO 验证
// ==========================================
void cpp_string_demo()
{
    std::cout << "\n--- std::string & SSO ---" << std::endl;

    // 初始化与拼接
    std::string s1 = "C++";
    std::string s2 = " Programming";
    std::string s3 = s1 + s2; // 运算符重载，非常直观

    std::cout << "Concatenated: " << s3 << std::endl;
    std::cout << "Length: " << s3.size() << std::endl;

    // 查找与子串
    // size_t 是无符号整数，std::string::npos 代表"没找到"
    size_t pos = s3.find("Prog");
    if (pos != std::string::npos)
    {
        std::cout << "Found 'Prog' at index: " << pos << std::endl;
        // 提取子串 (从 pos 开始，取 4 个字符)
        std::cout << "Substr: " << s3.substr(pos, 4) << std::endl;
    }

    // --- 拓展：验证 SSO (Small String Optimization) ---
    // C++11 之后，std::string 通常包含这个优化
    std::string shortStr = "Short";
    std::string longStr = "This is a very very very long string that will definitely go to heap";

    // data() 返回指向字符串内容的指针
    // 如果 data 的地址和对象本身的地址很近，说明在栈上(SSO)
    std::cout << "Address of shortStr obj: " << &shortStr << std::endl;
    // (void*) 强制转换是为了打印地址而不是字符串内容
    std::cout << "Address of shortStr data:" << (void *)shortStr.data() << " (Stack/Internal)" << std::endl;

    std::cout << "Address of longStr obj:  " << &longStr << std::endl;
    std::cout << "Address of longStr data: " << (void *)longStr.data() << " (Heap)" << std::endl;
}

// ==========================================
// 3. 最佳实践：传参优化 (const reference)
// ==========================================

// 【通用写法】适用于 C++98/11/14/17/20
// 使用 "const 引用" 避免拷贝。
// 这是在没有 string_view 时的标准最佳实践。
void printByRef(const std::string &str)
{
    std::cout << "[Ref] " << str << " (Size: " << str.size() << ")" << std::endl;
}

int main()
{
    c_style_demo();
    cpp_string_demo();

    std::cout << "\n--- Best Practices (Functions) ---" << std::endl;
    std::string myName = "Computer Science Student";

    // 1. 传递 std::string 对象
    printByRef(myName); // ? 高效：没有拷贝，直接引用

    // 2. 传递 C 风格字符串
    // 注意：这里会发生一次隐式转换 (char* -> string 临时对象)，会有一次内存分配
    // 在 C++17 之前，这是不可避免的代价
    printByRef("Hello");

    return 0;
}