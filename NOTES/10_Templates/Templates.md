[MindMap](./Templates.png)


# C++ 模板编程详解 (Templates & Generic Programming)

## 1. 核心本质：写“生成代码”的代码

模板不是最终的 C++ 代码，它是给编译器的**“蓝图” (Blueprint)** 或 **“规则”**。

* **编译器行为**：当你写了一个模板函数 `Print<T>`，编译器**不会**立即生成机器码。只有当你调用 `Print<int>(5)` 时，编译器才会根据模板，“照猫画虎”地生成一个专门处理 `int` 的函数版本。这个过程叫 **模板实例化 (Template Instantiation)**。
* **不存在的虚空**：如果你定义了一个模板但从未在代码中使用它，编译器在编译时甚至可能不会检查它的某些语法错误，因为它根本就不存在于最终的二进制文件中。

## 2. 为什么比 Java/C# 的泛型更强？

* **Java/C#**：泛型通常基于“类型擦除”或基类引用（Object），在运行时进行类型检查，有性能损耗。
* **C++**：模板在**编译时**展开。`Vector<int>` 和 `Vector<float>` 是两个完全独立、互不相关的类。这意味着 C++ 模板生成的代码和手动为每个类型写一遍代码的**性能是一模一样的**（Zero Overhead）。

## 3. 关键概念解析

### 3.1 函数模板 (Function Templates)

* **类型推导 (Type Deduction)**：通常不需要写 `Print<int>(5)`，直接写 `Print(5)`，编译器会自动推导出 `T` 是 `int`。
* **用途**：避免为 `int`, `float`, `string` 分别写三个逻辑一模一样的 `Add` 函数。遵守 **DRY (Don't Repeat Yourself)** 原则。

### 3.2 类模板 (Class Templates)

* **应用**：这是 STL 容器（`std::vector`, `std::map`）的基础。
* **非类型模板参数 (Non-Type Template Parameters)**：这是 C++ 的杀手锏。你不仅可以传“类型”（如 `int`），还可以传“值”（如整数）。
* 例如：`template<typename T, int N> class Array`。
* `Array<int, 5>` 和 `Array<int, 10>` 是**两个完全不同的类型**。这允许你在**栈 (Stack)** 上创建固定大小的数组，完全避免堆内存分配（`new`），性能极高。



## 4. 实际开发中的“坑”与最佳实践

1. **实现必须写在头文件**：模板通常**不能**将声明放在 `.h`，实现放在 `.cpp`。因为编译器在编译 `.cpp` 时不知道模板会被用来实例化什么类型。**通常做法是全部写在 `.h` 文件中**。
2. **编译时间膨胀**：因为编译器要为每一个不同的 `T` 生成一份新代码，过度使用模板会导致生成的二进制文件变大，且编译速度变慢。
3. **报错天书**：模板报错信息通常极其冗长且难以理解（虽然现代编译器已优化），调试难度较高。

---

## 5. 代码实战笔记

```cpp
/*
 * 文件名: templates_demo.cpp
 * 描述: 演示函数模板、类模板以及非类型模板参数（栈数组实现）
 * 编译指令: g++ -o templates templates_demo.cpp -std=c++14
 */

#include <iostream>
#include <string>

using namespace std;

// ==========================================
// 1. 函数模板 (Function Template)
// ==========================================
// T 是一个占位符，将来会被 int, float, string 等替换
template<typename T>
void Print(T value) {
    cout << "[Print] Value: " << value << endl;
}

// 模板也可以重载，甚至特化（针对特定类型做特殊处理）
// 这里演示一个简单的通用交换函数
template<typename T>
void MySwap(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// ==========================================
// 2. 类模板与非类型参数 (Class Template & Non-Type Param)
// ==========================================
// 这是一个极简版的 std::array
// T: 数组存储的数据类型
// N: 数组的大小 (编译期常量)
template<typename T, int N>
class StackArray {
private:
    T m_Data[N]; // 实际上是在栈上分配内存: int m_Data[5];

public:
    int GetSize() const { return N; }

    // 设置值
    void Set(int index, T value) {
        if (index >= 0 && index < N)
            m_Data[index] = value;
    }

    // 获取值
    T Get(int index) const {
        if (index >= 0 && index < N)
            return m_Data[index];
        return T(); // 返回默认值
    }

    // 打印所有元素
    void PrintAll() const {
        cout << "[StackArray<" << N << ">] ";
        for (int i = 0; i < N; i++) {
            cout << m_Data[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    cout << "=== 1. Function Template Demo ===" << endl;
    
    // 隐式推导 (Implicit Deduction)
    Print(5);           // 编译器自动生成 void Print(int)
    Print("Hello");     // 编译器自动生成 void Print(const char*)
    Print(5.5f);        // 编译器自动生成 void Print(float)

    // 显式指定 (Explicit Specification)
    // 强制编译器生成 void Print(int)，即使传入的是 float（会被截断）
    Print<int>(9.9); 

    int x = 10, y = 20;
    MySwap(x, y);
    cout << "Swapped: x=" << x << ", y=" << y << endl;


    cout << "\n=== 2. Class Template Demo ===" << endl;

    // 实例化一个存储 5 个整数的数组
    // 注意：StackArray<int, 5> 是一个完整的类型名
    StackArray<int, 5> intArray;
    
    for(int i = 0; i < 5; i++) {
        intArray.Set(i, i * 10);
    }
    intArray.PrintAll();

    // 实例化一个存储 3 个字符串的数组
    StackArray<string, 3> stringArray;
    stringArray.Set(0, "C++");
    stringArray.Set(1, "Templates");
    stringArray.Set(2, "Rocks");
    stringArray.PrintAll();

    // 验证：N 是编译期确定的，不能用变量
    // int size = 10;
    // StackArray<int, size> badArray; // ? 编译错误！N 必须是编译期常量 (constexpr)

    return 0;
}

```

### 学习要点总结 (Key Takeaways)

1. **代码生成器**：把模板想象成一个超级精密的“复印机”，给它什么类型，它就给出什么代码。
2. **`.h` 文件原则**：如果要把模板类的声明和实现分开，请确保它们都在头文件中（或者通过 `.tpp` 文件 include 到头文件末尾），否则链接器会报错。
3. **栈的高效**：`StackArray<int, 5>` 的例子展示了如何利用模板在栈上控制内存布局。这比 `std::vector`（堆分配）要快得多，但代价是空间有限且大小不可变。