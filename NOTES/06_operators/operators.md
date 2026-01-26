[MindMap](./operations.png)


# C++ 运算符与运算符重载 (Operators & Overloading)

## 1. 核心本质：运算符即函数

在 C++ 中，`1 + 2` 看起来是数学运算，但在底层，它实际上是一次函数调用。

* **对于内置类型**（`int`, `float`）：编译器直接生成对应的 CPU 指令（如 `ADD` 指令）。
* **对于自定义类型**（类/结构体）：运算符只是一个**函数名**比较特殊的函数。
* `a + b` 等价于 `a.operator+(b)` （作为成员函数时）
* 或者 `operator+(a, b)` （作为全局函数时）



## 2. 为什么要重载？ (Why & When)

* **目的**：让代码更直观、更具可读性。
* *Bad*: `Vector2 result = v1.Add(v2).Multiply(5);`
* *Good*: `Vector2 result = (v1 + v2) * 5;`


* **适用场景**：数学对象（向量、矩阵、复数）、字符串拼接、智能指针（重载 `*` 和 `->`）、流操作（`<<` 和 `>>`）。
* **警示**：不要为了“炫技”而重载。如果行为不符合直觉（例如重载 `+` 来执行减法，或者重载 `^` 做幂运算——因为它在 C++ 中是异或），会严重破坏代码可维护性。

## 3. 实现细节与关键考点

### 3.1 成员函数 vs. 全局(非成员)函数

* **成员函数重载**：运算符左侧的操作数必须是该类的对象。
* 例如：`Vector2::operator+(const Vector2& other)`
* 调用：`v1 + v2` 变成 `v1.operator+(v2)`，`this` 指针指向 `v1`。


* **全局函数重载**：通常用于左侧操作数不是你的类的情况。
* 典型案例：`operator<<` (流插入)。因为 `cout << v1` 中，左侧是 `std::ostream` 对象，你无法修改 C++ 标准库的 `ostream` 类源码，所以只能定义为全局函数：`operator<<(ostream&, const Vector2&)`。



### 3.2 深度解析：重载 `<<` (流插入运算符)

这是大一新生最容易卡住的地方。

* **为什么需要 `friend`（友元）？**
* 为了让全局的 `operator<<` 函数能够访问你类的**私有成员**（如 `x`, `y`），你需要在类内部声明它是“朋友”。


* **为什么要返回 `ostream&`？**
* 为了支持**链式调用**。
* `cout << v1 << v2 << endl;` 实际上执行顺序是 `((cout << v1) << v2) << endl;`。如果不返回引用，后续的 `<<` 将无法接收到流对象。



### 3.3 性能优化 (Best Practices)

* **传参**：始终使用 `const Reference` (如 `const Vector2& other`)，避免不必要的对象拷贝。
* **Const 修饰**：对于加减乘除比较等**不修改自身**的操作，成员函数必须标记为 `const`。
* `Vector2 operator+(...) const;`



---

## 4. 代码实战笔记

```cpp
/*
 * 文件名: operator_overloading.cpp
 * 描述: 演示 C++ 运算符重载的实现，包括算术运算、比较运算和流输出
 * 核心对象: Vector2 (二维向量)
 */

#include <iostream>

using namespace std;

// 定义一个简单的二维向量类
class Vector2 {
private:
    float x, y;

public:
    // 构造函数
    Vector2(float x, float y) : x(x), y(y) {}

    // ==========================================
    // 1. 算术运算符重载 (+)
    // ==========================================
    // 语法: 返回类型 operator符号 (参数) const
    // 注意: 加了 const，表示由于加法不改变当前的两个数，只产生新结果
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    // ==========================================
    // 2. 乘法运算符重载 (*) - 向量乘以标量
    // ==========================================
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    // ==========================================
    // 3. 比较运算符重载 (==, !=)
    // ==========================================
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2& other) const {
        // 复用已经写好的 == 操作符，避免逻辑重复
        return !(*this == other);
    }

    // ==========================================
    // 4. 流插入运算符重载 (<<) - 关键难点
    // ==========================================
    // 必须定义为非成员函数（通常设为友元以访问私有成员）
    // 左操作数是 ostream (cout)，右操作数是 Vector2
    friend ostream& operator<<(ostream& stream, const Vector2& vector);
};

// 全局函数实现 operator<<
// 返回 ostream& 是为了支持链式调用: cout << v1 << v2;
ostream& operator<<(ostream& stream, const Vector2& vector) {
    stream << "(" << vector.x << ", " << vector.y << ")";
    return stream; // 返回流对象本身
}

int main() {
    Vector2 position(10.0f, 20.0f);
    Vector2 speed(2.5f, 2.5f);
    Vector2 speed2(2.5f, 2.5f);

    cout << "--- 1. Arithmetic Operators Demo ---" << endl;
    // 这里的 + 实际上调用了 position.operator+(speed)
    Vector2 result = position + speed; 
    
    // 这里的 * 实际上调用了 result.operator*(2.0f)
    Vector2 doubled = result * 2.0f;

    // 这里的 << 调用了全局函数 operator<<(cout, doubled)
    cout << "Position: " << position << endl;
    cout << "Result:   " << result << endl;
    cout << "Doubled:  " << doubled << endl;

    cout << "\n--- 2. Comparison Operators Demo ---" << endl;
    if (speed == speed2) {
        cout << "Speed and Speed2 are equal!" << endl;
    }

    if (position != speed) {
        cout << "Position is different from Speed." << endl;
    }

    return 0;
}

```

### 学习要点总结

1. **本质**：`a + b` 只是 `operator+(a, b)` 的语法糖。
2. **`operator<<`**：必须写在类外（或作为友元），并且要返回 `ostream&` 引用以支持链式打印。
3. **`const` 正确性**：如果操作符不修改对象本身（如 `+`, `==`），务必加上 `const` 修饰函数。