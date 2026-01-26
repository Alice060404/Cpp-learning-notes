[Previous Part](./shallow_deep_copy.md)


在 C++ 中，**拷贝赋值运算符**（`operator=`）比**拷贝构造函数**复杂一点。

原因在于：拷贝构造函数是为一个**正在创建**的对象初始化，而赋值运算符是在为**已经存在**的对象赋新值。这意味着该对象可能已经占用了某些内存，你必须先处理掉旧的内存，否则就会造成**内存泄漏**。

---

## 1. 编写赋值运算符的三个核心步骤

要写出一个健壮的深拷贝赋值运算符，通常需要遵循以下逻辑：

1. **自赋值检查 (Self-assignment Check)：** 检查是否自己在给自己赋值（如 `a = a;`）。如果不检查，在释放旧内存时，你会把等号右边的数据也一起删了。
2. **释放原有资源：** 防止内存泄漏。
3. **分配新资源并拷贝数据：** 实现深拷贝。
4. **返回对象引用：** 为了支持链式赋值（如 `a = b = c;`）。

---

## 2. 代码实现

我们继续使用 `Deep` 类来演示：

```cpp
#include <iostream>
#include <cstring>

class Deep {
public:
    char* data;

    Deep(const char* str = "") {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }

    // 拷贝构造函数 (上一次讲过)
    Deep(const Deep& source) {
        data = new char[strlen(source.data) + 1];
        strcpy(data, source.data);
    }

    // --- 重点：拷贝赋值运算符 ---
    Deep& operator=(const Deep& source) {
        // 1. 自赋值检查
        if (this == &source) {
            return *this; 
        }

        // 2. 释放旧的内存，防止泄漏
        delete[] data;

        // 3. 申请新内存并复制内容
        data = new char[strlen(source.data) + 1];
        strcpy(data, source.data);

        // 4. 返回当前对象的引用
        return *this;
    }

    ~Deep() {
        delete[] data;
    }
};

int main() {
    Deep a("Hello");
    Deep b("World");

    b = a; // 触发拷贝赋值运算符
    
    std::cout << b.data << std::endl; // 输出 Hello
    return 0;
}

```

---

## 3. 进阶技巧：Copy-and-Swap（拷贝并交换）

在实际工程中，上面的写法虽然正确，但不是**异常安全**的（如果 `new` 申请内存失败抛出异常，原本的 `data` 已经被删除了，对象会处于损坏状态）。

```cpp
#include <algorithm> // 包含 std::swap

Deep& operator=(Deep source) { // 1. 注意这里是“按值传递”，自动调用了拷贝构造函数生成副本
    std::swap(data, source.data); // 2. 交换副本和当前对象的内容
    return *this;                 // 3. 函数结束，副本 source 自动析构，顺便带走了旧内存
}

```

* **优点：** 极其简洁，天然处理了自赋值，且具有异常安全性。

---

## 总结：三法则 (Rule of Three)

现在你已经集齐了这三样东西：

1. **析构函数** (释放资源)
2. **拷贝构造函数** (从无到有实现深拷贝)
3. **拷贝赋值运算符** (从有到有实现深拷贝)

**记住：只要你的类里用了 `new`，这三者必须同时出现。**