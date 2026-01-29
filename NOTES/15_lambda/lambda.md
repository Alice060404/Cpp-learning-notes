[MindMap](./lambda.png)


# C++ Lambda 表达式详解 (Lambda Expressions)

## 1. 核心本质：匿名函数对象

思维导图中提到它是“不需要正式定义的快速丢弃函数”。

* **编译器视角**：当你写一个 Lambda 时，编译器在幕后实际上生成了一个**匿名类 (Anonymous Class)**，并重载了 `operator()`。
* **闭包 (Closure)**：Lambda 不仅仅是代码，它还能**捕获**定义它时的上下文变量（Context）。这种“代码 + 环境”的组合被称为闭包。

## 2. 语法解剖 (Anatomy)

标准语法如下：
`[捕获列表](参数列表) mutable -> 返回类型 { 函数体 }`

1. **`[]` 捕获列表 (Capture Clause)**：**这是 Lambda 的灵魂**。它决定了 Lambda 内部能访问外部哪些变量，以及如何访问（拷贝还是引用）。
2. **`()` 参数列表 (Parameters)**：和普通函数参数一样。如果不需要参数，可以省略 `()` (但在 C++23 前通常保留)。
3. **`mutable` 修饰符 (可选)**：默认情况下，按值捕获的变量在 Lambda 内部是**只读 (const)** 的。加上 `mutable` 后，允许在 Lambda 内部修改这些副本。
4. **`->` 返回类型 (可选)**：通常编译器能自动推导，但如果逻辑复杂，建议显式写出 `-> int`。

## 3. 捕获列表详解：控制权的艺术

这是新手最容易晕的地方，也是面试高频考点。

| 捕获方式 | 语法 | 含义 | 风险/特性 |
| --- | --- | --- | --- |
| **空捕获** | `[]` | 不捕获任何外部变量 | 只能使用局部参数和全局变量 |
| **按值全捕获** | `[=]` | 拷贝外部所有可见变量 | 安全，但有拷贝开销。**副本是 const 的** |
| **按引用全捕获** | `[&]` | 引用外部所有可见变量 | 高效，但有**悬空引用 (Dangling Reference)** 风险 |
| **混合捕获** | `[=, &x]` | 默认按值，但 `x` 按引用 | 精细控制 |
| **混合捕获** | `[&, x]` | 默认按引用，但 `x` 按值 | 精细控制 |
| **this 捕获** | `[this]` | 捕获当前类对象的指针 | 允许 Lambda 访问类的成员变量 |

> **致命陷阱**：绝对不要在返回一个 Lambda 的同时，使用 `[&]` 捕获局部变量。因为函数结束后局部变量销毁，Lambda 拿到的引用变成了“野指针”。

## 4. `mutable` 的作用

为什么需要 `mutable`？

* 当我们使用 `[=]` 按值捕获变量 `x` 时，编译器生成的匿名类中，成员变量 `x` 是被 `const` 修饰的。
* 如果你想在 Lambda 内部修改这个**副本**（注意：不会影响外部原本的变量），必须加上 `mutable` 关键字。

---

## 5. 代码实战笔记

```cpp
/*
 * 文件名: lambda_deep_dive.cpp
 * 描述: 演示 Lambda 的基本语法、捕获列表的区别、mutable 的原理及 STL 配合使用
 * 编译: g++ -o lambda_test lambda_deep_dive.cpp -std=c++14
 */

#include <iostream>
#include <vector>
#include <algorithm> // for std::find_if, std::for_each
#include <functional> // for std::function

using namespace std;

// ==========================================
// 1. 基础语法与 std::function
// ==========================================
void BasicDemo() {
    cout << "\n--- 1. Basic Syntax ---" << endl;

    // 定义一个最简单的 Lambda
    auto sayHello = []() {
        cout << "  Hello from Lambda!" << endl;
    };
    sayHello();

    // 带参数的 Lambda
    // auto 自动推导类型，实际上它是一个编译器生成的 unique closure type
    auto add = [](int a, int b) -> int {
        return a + b;
    };
    cout << "  Add(5, 3): " << add(5, 3) << endl;

    // 使用 std::function 存储 Lambda (适合作为回调参数传递)
    // 语法: std::function<返回类型(参数类型)>
    std::function<int(int)> square = [](int x) { return x * x; };
    cout << "  Square(4): " << square(4) << endl;
}

// ==========================================
// 2. 捕获列表与 mutable (Captures)
// ==========================================
void CaptureDemo() {
    cout << "\n--- 2. Captures & Mutable ---" << endl;

    int x = 100;
    int y = 200;

    // [=] 按值捕获 (Read-only Copy)
    // 这里的 x 是外部 x 的一份拷贝，且默认是 const
    auto captureByValue = [=]() {
        // x++; //  编译错误！x 是只读的
        cout << "  [Value] x: " << x << endl;
    };
    captureByValue();

    // [=] + mutable (Modifiable Copy)
    // 允许修改拷贝进来的 x，但不会影响外部的 x
    auto captureMutable = [=]() mutable {
        x++; //  合法，修改的是 Lambda 内部的副本
        cout << "  [Mutable] x internal: " << x << endl;
    };
    captureMutable();
    cout << "  [Mutable] x external: " << x << " (Unchanged)" << endl;

    // [&] 按引用捕获 (Read/Write Reference)
    // 直接操作外部变量，不需要 mutable
    auto captureByRef = [&]() {
        y++; //  修改了外部的 y
        cout << "  [Ref] y internal: " << y << endl;
    };
    captureByRef();
    cout << "  [Ref] y external: " << y << " (Changed!)" << endl;
}

// ==========================================
// 3. 实战：结合 STL 算法
// ==========================================
void AlgorithmDemo() {
    cout << "\n--- 3. STL Algorithms ---" << endl;

    vector<int> numbers = {1, 5, 8, 9, 12, 4, 7};
    int threshold = 6;

    // 场景：找到第一个大于 threshold 的数
    // 以前：需要写一个循环或者单独定义一个函数
    // 现在：直接在 find_if 里面写逻辑，利用捕获列表传入 threshold
    
    auto it = std::find_if(numbers.begin(), numbers.end(), [threshold](int val) {
        return val > threshold; // 捕获了外部的 6
    });

    if (it != numbers.end()) {
        cout << "  First number > " << threshold << " is: " << *it << endl;
    }

    // 场景：打印所有元素
    cout << "  Elements: ";
    std::for_each(numbers.begin(), numbers.end(), [](int val) {
        cout << val << " ";
    });
    cout << endl;
}

int main() {
    BasicDemo();
    CaptureDemo();
    AlgorithmDemo();
    return 0;
}

```

### 学习要点 (Key Takeaways)

1. **就地定义**：Lambda 最大的价值在于**“逻辑局部化”**。当这个函数逻辑只在这里用一次时，不要去污染全局命名空间，直接写个 Lambda。
2. **默认按值捕获**：为了安全，尽量优先使用 `[=]` 或显式捕获 `[x]`。只有当你确实需要修改外部变量，或者拷贝成本巨大（如大容器）时，才使用引用捕获 `[&]`。
3. **看懂报错**：如果你在 Lambda 里写了赋值语句却报错“assigning to variable in a lambda is a constant expression”，那就是忘了加 `mutable`。