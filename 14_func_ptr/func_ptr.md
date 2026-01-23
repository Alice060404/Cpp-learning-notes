[MindMap](./func_ptr.png)


# C++ 函数指针与 Lambda 表达式详解 (Function Pointers & Lambdas)

## 1. 核心本质：指向代码段的指针

我们通常理解的指针（如 `int*`）指向的是**堆或栈上的数据**。
而函数指针指向的是**代码段 (Code Segment)** 中的机器指令。

* **物理意义**：函数名在编译后，本质上就是一个内存地址（该函数第一条指令的地址）。
* **作用**：将“函数”赋值给变量，或者作为参数传递给另一个函数。这是实现 **回调 (Callback)** 机制的基础。

## 2. 语法噩梦与解药

### 2.1 原始语法 (The Ugly C-Style)

阅读规则：**从里向外读**。

```cpp
// 声明一个变量 funcPtr
// 它是一个指针 (*funcPtr)
// 它指向的函数接受两个 int 参数 (int, int)
// 它指向的函数返回 void
void (*funcPtr)(int, int); 

```

* **坑点**：`void *funcPtr(int, int)` —— 这不是函数指针，这是声明了一个函数，返回 `void*`。括号 `(*funcPtr)` 至关重要。

### 2.2 现代解药 (Modern C++ Aliases)

使用 `typedef` 或 C++11 的 `using` 关键字，可以让代码像人类语言。

* **推荐写法**：
```cpp
using Action = void(*)(int); // 定义类型别名
Action myFunc = &SomeFunction;

```



## 3. 核心用途：回调与解耦

为什么我们需要把函数当参数传？

* **解耦 (Decoupling)**：编写一个通用的处理函数（如 `Sort` 或 `ForEach`），但具体的处理逻辑（是升序还是降序？是打印还是求和？）由调用者决定。
* **系统 API**：Windows API 或 Linux 信号处理大量使用函数指针来通知应用程序“某事发生了”。

## 4. 进阶：现代 C++ 的替代方案

虽然函数指针是基础，但现代 C++ (C++11/14/17) 提供了更强大的工具：

1. **Lambda 表达式 (Anonymous Functions)**：
* **定义**：`[]() { ... }`
* **优势**：内联定义，代码紧凑；支持 **捕获 (Capture)** 上下文变量（这是裸函数指针做不到的）。


2. **`std::function`**：
* **定义**：`#include <functional>`
* **优势**：万能包装器。它可以存储函数指针、Lambda、仿函数（Functor）等任何“可调用对象”。
* **代价**：比裸指针稍微重一点（有一定的运行时开销）。



---

## 5. 代码实战笔记

```cpp
/*
 * 文件名: function_pointers_lambda.cpp
 * 描述: 演示原始函数指针、回调机制以及现代 Lambda 的演进
 * 编译: g++ -o func_ptr function_pointers_lambda.cpp -std=c++14
 */

#include <iostream>
#include <vector>
#include <functional> // for std::function

using namespace std;

// ==========================================
// 1. 原始函数指针基础
// ==========================================
void Hello(int a) {
    cout << "  Hello Value: " << a << endl;
}

void World(int a) {
    cout << "  World Value: " << a << endl;
}

// ==========================================
// 2. 回调机制 (Callback Mechanism)
// ==========================================
// 这是一个通用的遍历函数。
// 它不知道要对数据做什么，它只负责遍历。
// "做什么" 由参数 func 决定。
void ForEach(const vector<int>& values, void(*func)(int)) {
    for (int value : values) {
        func(value); // 调用回调函数
    }
}

// 使用现代 C++ 的 std::function，支持 Lambda 和更多类型
void ForEachModern(const vector<int>& values, const std::function<void(int)>& func) {
    for (int value : values) {
        func(value);
    }
}

int main() {
    cout << "=== 1. Basic Function Pointers ===" << endl;
    
    // 声明一个函数指针变量
    // auto func = Hello; // 最简单的写法 (类型推导)
    void(*func)(int) = Hello; // 显式写法
    
    // 调用
    func(10); // 输出 Hello Value: 10
    
    // 重新赋值（像普通变量一样）
    func = World;
    func(20); // 输出 World Value: 20


    cout << "\n=== 2. Callbacks (Decoupling Logic) ===" << endl;
    vector<int> nums = {1, 2, 3, 4, 5};
    
    // 将函数作为参数传递
    cout << ">> Using function pointer:" << endl;
    ForEach(nums, Hello); // 对每个元素调用 Hello


    cout << "\n=== 3. Lambda Expressions (Modern C++) ===" << endl;
    
    // 场景：我不想专门为此写一个全局函数，我想直接在这里写逻辑
    // 语法: [](参数) { 实现 }
    
    cout << ">> Using Lambda with ForEach:" << endl;
    ForEach(nums, [](int val) {
        cout << "  Lambda Value: " << val * 10 << endl;
    });


    cout << "\n=== 4. Lambda Captures (The Killer Feature) ===" << endl;
    
    int multiplier = 100;
    
    // 这里的 Lambda 捕获了外部变量 multiplier
    // 原始的 void(*)(int) 函数指针是做不到这一点的！
    // 必须使用支持 std::function 的版本
    
    ForEachModern(nums, [multiplier](int val) {
        // [=] 表示按值捕获所有外部变量
        // [&] 表示按引用捕获
        cout << "  Captured Multiply: " << val * multiplier << endl;
    });

    return 0;
}

```

### 学习要点总结 (Key Takeaways)

1. **区分声明与调用**：`void (*func)()` 是声明指针，`void *func()` 是声明返回指针的函数。
2. **Lambda 是未来**：在 90% 的情况下，现代 C++ 开发会优先使用 Lambda 表达式配合 `<algorithm>` 库（如 `std::sort`, `std::for_each`），而不是手写函数指针。
3. **捕获列表 `[]**`：这是 Lambda 最强大的地方，它允许函数拥有“状态”（Closure），打破了传统函数的作用域限制。