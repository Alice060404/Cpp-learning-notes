[MindMap](./iterators.png)


# C++ 核心笔记：迭代器 (Iterators)

## 1. 核心概念与设计哲学

在 C++ 中，**迭代器 (Iterator)** 是一种**行为类似指针**的对象，它的核心目的是为访问容器（如 `vector`, `list`, `map`）中的元素提供一种**统一的接口**。

### 为什么需要迭代器？

* **统一访问**：数组内存是连续的，可以用下标 `arr[i]` 访问。但链表（List）、集合（Set）、哈希表（Map）的内存是不连续的，无法通过下标高效访问。迭代器屏蔽了底层数据结构的差异，让你用同样的代码遍历不同的容器。
* **算法复用**：STL 中的算法（如 `std::sort`, `std::find`）不知道容器的存在，它们只接受迭代器。这使得一套算法可以应用于所有支持迭代器的容器。

### 核心操作（类似指针）

* **解引用 (`*it`)**：获取迭代器当前指向的元素值。
* **成员访问 (`it->`)**：访问元素的成员（常用于 Map，如 `it->first`）。
* **递增 (`++it`)**：移动到下一个元素。
* **比较 (`it != end`)**：判断是否到达序列末尾。

## 2. 关键区间：左闭右开 `[begin, end)`

这是 C++ 迭代器设计中最重要、也最容易让新手困惑的概念：

* **`begin()`**：指向容器的**第一个元素**。
* **`end()`**：指向容器**最后一个元素之后的位置**（Past-the-end）。
* *注意*：`end()` **不指向**最后一个有效元素！对 `end()` 进行解引用是未定义行为（会导致程序崩溃）。
* 空容器：`begin() == end()`。



## 3. 迭代方式的演变

### 3.1 传统方式 (C++98)

显式声明迭代器类型，代码冗长。

```cpp
std::vector<int>::iterator it = vec.begin();

```

### 3.2 现代方式 (C++11 Range-based For Loop)

语法糖，编译器自动处理 `begin()` 和 `end()`。简洁、不易出错。

```cpp
for (int val : vec) { ... }

```

### 3.3 结构化绑定 (C++17 Structured Binding)

这是处理 `std::map` 或 `std::unordered_map` 的神器。可以直接将键值对解包，不再需要写繁琐的 `it->first`, `it->second`。

```cpp
for (auto [key, value] : myMap) { ... }

```

## 4. 迭代器类型与层级

虽然语法通用，但不同容器的迭代器能力不同（编译期决定）：

1. **随机访问迭代器 (Random Access)**：支持 `it + n`，`it[n]`。
* *容器*：`vector`, `deque`, `array`。


2. **双向迭代器 (Bidirectional)**：支持 `++`, `--`，不支持 `+ n`。
* *容器*：`list`, `set`, `map`。


3. **前向迭代器 (Forward)**：只能 `++`。
* *容器*：`forward_list`。



**特殊类型**：

* `const_iterator`：只读迭代器，解引用后无法修改值（`*it` 是 `const` 的）。
* `reverse_iterator`：反向迭代器 (`rbegin()`, `rend()`)，`++` 操作实际上是向前移动。

## 5. 高级话题：迭代器失效 (Iterator Invalidation)

这是面试和实战中的**高频考点/Bug源头**。
当你在这个容器中**添加**或**删除**元素时，原有的迭代器可能会变成“野指针”。

* **Vector 扩容**：如果你 `push_back` 导致 vector 重新分配内存，**所有**指向旧内存的迭代器瞬间失效。
* **删除元素**：在 `for` 循环中直接 `erase(it)` 会导致 `it` 失效，下一次循环 `it++` 就会崩溃。
* *正确做法*：`it = container.erase(it);` （erase 会返回下一个有效的迭代器）。



---

## 附录：C++ 代码示例

以下代码涵盖了 Vector 的遍历、Map 的 C++17 结构化绑定遍历，以及**最关键的**“如何在遍历中安全删除元素”。

**文件名**: `IteratorDemo.cpp`

```cpp
#include <iostream>
#include <vector>
#include <map>
#include <string>

int main() {
    // ==========================================
    // 1. Vector 遍历方式演变
    // ==========================================
    std::cout << "--- Vector Iteration ---" << std::endl;
    std::vector<int> scores = {90, 85, 70, 95, 60};

    // [方式 A]: 传统显式迭代器 (C++98/03)
    // 优点：最灵活，可以控制具体位置
    std::cout << "1. Traditional: ";
    for (std::vector<int>::iterator it = scores.begin(); it != scores.end(); ++it) {
        // *it 获取值，可以修改
        if (*it < 60) *it = 60; // 修改值
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // [方式 B]: 范围 For 循环 (C++11) - 推荐日常使用
    // const auto& 避免拷贝，且保证不修改原数据
    std::cout << "2. Range-based (C++11): ";
    for (const auto& score : scores) {
        std::cout << score << " ";
    }
    std::cout << std::endl;

    // [方式 C]: 反向迭代器
    std::cout << "3. Reverse Iterator: ";
    for (auto rit = scores.rbegin(); rit != scores.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << std::endl;


    // ==========================================
    // 2. Map 遍历与 C++17 结构化绑定
    // ==========================================
    std::cout << "\n--- Map Iteration ---" << std::endl;
    std::map<std::string, int> ageMap;
    ageMap["Alice"] = 20;
    ageMap["Bob"] = 22;
    ageMap["Charlie"] = 19;

    // [方式 A]: 传统方式 (使用 ->first, ->second)
    std::cout << "1. Traditional Map:\n";
    for (auto it = ageMap.begin(); it != ageMap.end(); ++it) {
        // it 类似于指向 pair<const string, int> 的指针
        std::cout << "   " << it->first << ": " << it->second << std::endl;
    }

    // [方式 B]: C++17 结构化绑定 (Structured Binding) - 最优雅
    // 直接将 pair 解包为 key 和 value
    std::cout << "2. Structured Binding (C++17):\n";
    for (auto [name, age] : ageMap) {
        std::cout << "   " << name << ": " << age << std::endl;
    }


    // ==========================================
    // 3. 进阶：如何安全地“边遍历边删除”
    // ==========================================
    std::cout << "\n--- Safe Erasure (Iterator Invalidation) ---" << std::endl;
    std::vector<int> data = {1, 2, 3, 4, 5, 6};

    // 任务：删除所有偶数
    // 错误写法：for (auto it = data.begin(); it != data.end(); ++it) { if (...) data.erase(it); } 
    // 原因：erase(it) 后 it 失效，下一次 ++it 会崩溃。

    // 正确写法：
    for (auto it = data.begin(); it != data.end(); /* 留空 */) {
        if (*it % 2 == 0) {
            std::cout << "Erasing " << *it << std::endl;
            // erase 返回指向被删元素之后的下一个元素的迭代器
            it = data.erase(it); 
        } else {
            // 只有没删除时才手动 ++
            ++it; 
        }
    }

    std::cout << "Remaining data: ";
    for (int v : data) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}

```