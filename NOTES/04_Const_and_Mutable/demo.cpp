/**
 * @file const_mutable_demo.cpp
 * @brief 深度演示 Const 指针规则、Const 成员函数以及 Mutable 在缓存/Lambda 中的应用
 * @note 适合复制到本地 IDE (如 VS Code, CLion) 运行调试
 */

#include <iostream>
#include <string>

// ==========================================
// 1. 演示 Mutable 与 逻辑常量性 (Logical Constness)
// ==========================================
class BigDataCalculator
{
private:
    int data;

    // 【重点】mutable 关键字
    // 即使在 const 函数中，这些变量也可以被修改
    // 场景：缓存 (Caching) - 这里的修改不影响对象的"核心状态"
    mutable int cachedResult;
    mutable bool isCached;

    // 场景：调试/统计
    mutable int accessCount;

public:

    //初始化列表
    BigDataCalculator(int val) : data(val), cachedResult(0), isCached(false), accessCount(0) {}

    // 这是一个 const 函数：承诺不修改对象的"核心数据" (data)
    int getComplexResult() const
    {
        // 修改 mutable 变量是合法的
        accessCount++;

        if (isCached)
        {
            std::cout << "[Log] Cache hit! Access count: " << accessCount << std::endl;
            return cachedResult;
        }

        std::cout << "[Log] Calculating heavy task... Access count: " << accessCount << std::endl;

        // 模拟耗时计算，并更新 mutable 缓存
        cachedResult = data * data * 10;
        isCached = true;

        return cachedResult;
    }

    // 普通 Setter，不能是 const
    void setData(int val)
    {
        data = val;
        isCached = false; // 数据变了，缓存失效
    }
};

// ==========================================
// 2. 演示 Lambda 中的 Mutable
// ==========================================
void lambdaDemo()
{
    std::cout << "\n--- Lambda Mutable Demo ---" << std::endl;
    int x = 10;

    // [=] 按值捕获：x 在 Lambda 内部是外部 x 的一份拷贝
    // 默认情况下，Lambda 的 operator() 是 const 的，不允许修改拷贝
    auto lambda = [x]() mutable
    {
        x++; // ? 加上 mutable 后允许修改"拷贝"
        std::cout << "Inside Lambda: x = " << x << std::endl;
    };

    lambda();                                                                // 输出 11
    std::cout << "Outside Lambda: x = " << x << " (Unchanged)" << std::endl; // 输出 10
}

// ==========================================
// 3. 演示 Const 指针 (The Tricky Part)
// ==========================================
void pointerDemo()
{
    std::cout << "\n--- Const Pointer Demo ---" << std::endl;
    int a = 10;
    int b = 20;

    // [Case A] 指向常量的指针 (Pointer to Const)
    // 记忆：const 在 * 左边 -> 内容是 const
    const int *ptr1 = &a;
    // *ptr1 = 30;  // ? 错误：Read-only value (你不能通过 ptr1 修改 a)
    ptr1 = &b; // ? 允许：ptr1 可以指向别的地方
    std::cout << "ptr1 pointed to a, now points to b." << std::endl;

    // [Case B] 常量指针 (Const Pointer)
    // 记忆：const 在 * 右边 -> 指针本身是 const
    int *const ptr2 = &a;
    *ptr2 = 30; // ? 允许：可以修改指向的值 (a 变成了 30)
    // ptr2 = &b;   // ? 错误：Cannot assign to variable 'ptr2' (指针不能改道)
    std::cout << "ptr2 changed a to: " << a << std::endl;
}

int main()
{
    // 1. 测试 Mutable 缓存机制
    std::cout << "--- Class Mutable Demo ---" << std::endl;

    // 注意：calc 对象本身是 const 的！
    // 这意味着我们只能调用它的 const 成员函数
    const BigDataCalculator calc(5);

    // 第一次调用：执行计算并缓存 (修改了 mutable 成员)
    std::cout << "Result 1: " << calc.getComplexResult() << std::endl;

    // 第二次调用：直接使用缓存 (再次修改了 mutable accessCount)
    std::cout << "Result 2: " << calc.getComplexResult() << std::endl;

    // calc.setData(10); // ? 错误：setData 不是 const 函数，const 对象不能调用

    // 2. 测试指针与 Lambda
    pointerDemo();
    lambdaDemo();

    return 0;
}