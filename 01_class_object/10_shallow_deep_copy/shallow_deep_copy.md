理解**浅拷贝（Shallow Copy）**与**深拷贝（Deep Copy）**是掌握 C++ 内存管理和类设计的关键一步。

在 C++ 中，当我们把一个对象赋值给另一个对象时（例如 `Object A = B;`），如果类中包含**指针**或**动态分配的内存**，这两者的区别就会直接决定你的程序是正常运行还是直接崩溃。

---

## 1. 核心定义

### 浅拷贝 (Shallow Copy)

* **行为：** 只是简单地将成员变量的值从一个对象复制到另一个对象。
* **指针的处理：** 如果类成员里有指针，浅拷贝只复制**指针的地址**，而不复制指针指向的内容。
* **结果：** 两个对象中的指针指向**同一块内存地址**。

### 深拷贝 (Deep Copy)

* **行为：** 不仅复制成员变量的值，还会为指针成员**重新分配一块独立的内存**，并将原地址的内容复制过去。
* **指针的处理：** 复制指针所指向的“数据”本身。
* **结果：** 两个对象拥有各自独立的内存空间，互不影响。

---

## 2. 为什么要区分？（浅拷贝的危机）

浅拷贝最致命的问题在于：

1. **修改干扰：** 修改对象 A 的数据，对象 B 的数据也跟着变了（因为它们共用一块内存）。
2. **双重释放 (Double Free)：** 当对象 A 生命周期结束调用析构函数释放内存后，对象 B 也结束生命周期。此时 B 试图释放同一块已经被释放的内存，程序会立即**崩溃（Segment Fault）**。

---

## 3. C++ 代码示例

让我们通过一个简单的 `MyString` 类来看看两者的区别。

### 场景 A：浅拷贝（系统默认行为）

如果你不写拷贝构造函数，C++ 默认会进行浅拷贝。

```cpp
#include <iostream>
#include <cstring>

class Shallow {
public:
    char* data;
    Shallow(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }
    // 使用默认拷贝构造函数 (浅拷贝)
    ~Shallow() {
        delete[] data; // 释放内存
    }
};

int main() {
    Shallow obj1("Hello");
    Shallow obj2 = obj1; // 浅拷贝：obj2.data 和 obj1.data 指向同一个地址

    // 程序结束时，obj2 析构释放了地址，obj1 析构时再次释放同一地址 -> 崩溃！
    return 0;
}

```

### 场景 B：深拷贝（手动实现）

为了安全，我们需要自己编写**拷贝构造函数**。

```cpp
#include <iostream>
#include <cstring>

class Deep {
public:
    char* data;

    // 构造函数
    Deep(const char* str) {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // 手写拷贝构造函数 (实现深拷贝)
    Deep(const Deep& source) {
        // 1. 为新对象申请独立的内存
        data = new char[strlen(source.data) + 1];
        // 2. 复制内容，而不是地址
        strcpy(data, source.data);
        std::cout << "触发深拷贝，申请了新内存" << std::endl;
    }

    // 析构函数
    ~Deep() {
        delete[] data;
    }
};

int main() {
    Deep obj1("Hello");
    Deep obj2 = obj1; // 触发深拷贝

    // 修改 obj1 不会影响 obj2
    obj1.data[0] = 'y'; 
    
    // 程序结束时，两个对象分别释放各自的内存，安全！
    return 0;
}

```

---

## 4. 总结与对比

| 特性 | 浅拷贝 (Shallow Copy) | 深拷贝 (Deep Copy) |
| --- | --- | --- |
| **内存分配** | 共享同一块内存 | 申请一块新的独立内存 |
| **复制内容** | 仅复制指针地址 | 复制指针指向的实际数据 |
| **默认情况** | C++ 默认生成的拷贝构造函数 | 必须由开发者手动编写 |
| **安全性** | 容易导致 Double Free 和数据篡改 | 安全，对象之间完全解耦 |
| **开销** | 极低（只复制几个字节的地址） | 较高（涉及内存申请和大数据搬运） |

---

## 5. 建议：三/五法则

在 C++ 学习中，有一个著名的 **“Rule of Three”（三法则）**：
如果你发现你的类需要手动写 **析构函数 (Destructor)** 来释放资源，那么你几乎一定也需要手动写 **拷贝构造函数 (Copy Constructor)** 和 **拷贝赋值运算符 (Copy Assignment Operator)**。

这是因为如果你需要手动释放内存，说明类中含有指针，而默认的浅拷贝无法处理指针的正确解耦。

[Next Part](./Copy_Assignment_Operator.md)