[MindMap](./smart_ptr.png)


# C++ 智能指针详解 (Smart Pointers)

## 1. 核心背景：为什么要用智能指针？

在传统的 C++ (`new`/`delete`) 中，程序员需要手动管理内存，这导致了两个噩梦：

1. **内存泄漏 (Memory Leak)**：`new` 了之后忘记 `delete`，或者因为异常跳过了 `delete`。
2. **悬空指针 (Dangling Pointer)**：内存已经被释放了，但指针还在指向它，再次访问会导致崩溃。

**智能指针** 是 RAII (资源获取即初始化) 机制的完美体现：它是一个**包装类 (Wrapper Class)**，行为像指针（重载了 `*` 和 `->`），但在析构函数中自动管理内存释放。

## 2. 三剑客解析

### 2.1 `std::unique_ptr` (独占指针) —— **默认首选**

* **特性**：**独占所有权**。同一时间内，只能有一个 `unique_ptr` 指向该对象。
* **机制**：
* **不可复制**：`unique_ptr<T> p2 = p1;` // ? 编译报错！
* **可移动**：`unique_ptr<T> p2 = std::move(p1);` // ? 所有权转移，p1 变为空。


* **性能**：几乎零开销（和裸指针一样快），不需要维护引用计数。
* **最佳实践**：优先使用 `std::make_unique<T>()` 创建（C++14），比 `new` 更异常安全。

### 2.2 `std::shared_ptr` (共享指针)

* **特性**：**共享所有权**。多个指针指向同一个对象，只有当**所有**指针都销毁或放弃所有权时，内存才会被释放。
* **机制**：**引用计数 (Reference Counting)**。
* 每多一个 `shared_ptr` 指向该对象，计数器 +1。
* 每析构一个 `shared_ptr`，计数器 -1。
* 计数器归零 -> 调用 `delete`。


* **底层开销**：
* 需要维护一个**控制块 (Control Block)** 来存储引用计数。
* 引用计数的操作是**原子操作 (Atomic)**（为了线程安全），有微小的性能损耗。


* **最佳实践**：使用 `std::make_shared<T>()`。它将“对象内存”和“控制块内存”一次性连续分配，减少内存碎片，提高缓存命中率。

### 2.3 `std::weak_ptr` (弱引用指针) —— **破局者**

* **痛点**：`shared_ptr` 存在**循环引用 (Cyclic Reference)** 问题。
* A 指向 B，B 指向 A。两者的引用计数永远是 1，永远不会归零，导致内存泄漏。
* **特性**：**不拥有所有权**。
* 它指向 `shared_ptr` 管理的对象，但**不增加**引用计数。
* 必须将 `shared_ptr` 赋值给它。
* 访问对象时，需要通过 `.lock()` 提升为 `shared_ptr`（因为对象可能已经没了）。



---

## 3. 代码实战笔记

```cpp
/*
 * 文件名: smart_pointers_demo.cpp
 * 描述: 演示 unique_ptr 的移动语义、shared_ptr 的引用计数及 weak_ptr 解决循环引用
 * 编译指令: g++ -o smart_ptr smart_pointers_demo.cpp -std=c++14
 */

#include <iostream>
#include <memory> // 必须包含头文件
#include <string>

using namespace std;

class Entity {
public:
    string m_Name;
    Entity(string name) : m_Name(name) {
        cout << "  [Construct] " << m_Name << endl;
    }
    ~Entity() {
        cout << "  [Destruct]  " << m_Name << endl;
    }
    void Print() { cout << "  -> Hello from " << m_Name << endl; }
};

// ==========================================
// 1. unique_ptr 演示
// ==========================================
void UniquePtrDemo() {
    cout << "\n--- 1. std::unique_ptr Demo ---" << endl;
    
    // 创建：使用 make_unique (推荐)
    unique_ptr<Entity> uPtr1 = make_unique<Entity>("UniqueEntity");
    uPtr1->Print();

    // 复制：禁止！
    // unique_ptr<Entity> uPtr2 = uPtr1; // <--- 编译错误

    // 移动：所有权转移
    cout << "  (Moving ownership from uPtr1 to uPtr2...)" << endl;
    unique_ptr<Entity> uPtr2 = std::move(uPtr1);

    if (!uPtr1) cout << "  uPtr1 is now empty (nullptr)." << endl;
    if (uPtr2)  cout << "  uPtr2 owns the object." << endl;

} // uPtr2 离开作用域，自动释放内存

// ==========================================
// 2. shared_ptr 演示
// ==========================================
void SharedPtrDemo() {
    cout << "\n--- 2. std::shared_ptr Demo ---" << endl;

    // 创建：使用 make_shared (推荐，为了内存连续性)
    shared_ptr<Entity> sPtr1 = make_shared<Entity>("SharedEntity");
    
    {
        cout << "  [Ref Count] Initial: " << sPtr1.use_count() << endl; // 1

        shared_ptr<Entity> sPtr2 = sPtr1; // 共享，计数 +1
        cout << "  [Ref Count] After copy: " << sPtr1.use_count() << endl; // 2
        
        sPtr2->Print();
    } // sPtr2 离开作用域，计数 -1

    cout << "  [Ref Count] Back in outer scope: " << sPtr1.use_count() << endl; // 1

} // sPtr1 离开作用域，计数归零 -> 释放内存

// ==========================================
// 3. weak_ptr 解决循环引用
// ==========================================
struct Node {
    string value;
    // 如果这里用 shared_ptr<Node>，就会发生循环引用内存泄漏！
    // 必须用 weak_ptr 打破环状依赖
    weak_ptr<Node> next; 
    
    Node(string v) : value(v) { cout << "  [Node Construct] " << value << endl; }
    ~Node() { cout << "  [Node Destruct]  " << value << endl; }
};

void WeakPtrCycleDemo() {
    cout << "\n--- 3. Cyclic Reference Fix (weak_ptr) ---" << endl;

    shared_ptr<Node> nodeA = make_shared<Node>("A");
    shared_ptr<Node> nodeB = make_shared<Node>("B");

    // A 指向 B，B 指向 A
    nodeA->next = nodeB;
    nodeB->next = nodeA;

    cout << "  Cycle created. If next was shared_ptr, destructors would NEVER run." << endl;
    
    // 检查 weak_ptr 指向的对象是否存在
    if (auto tempPtr = nodeA->next.lock()) {
        cout << "  Accessing B from A: " << tempPtr->value << endl;
    }
} // 函数结束，nodeA 和 nodeB 正常析构（因为 weak_ptr 不增加引用计数）

int main() {
    UniquePtrDemo();
    SharedPtrDemo();
    WeakPtrCycleDemo();
    return 0;
}

```

### 学习建议 (Tips for CS Freshman)

1. **原则**：默认使用 `std::unique_ptr`。只有当你确实需要多个拥有者共享资源时，才使用 `std::shared_ptr`。
2. **避免**：尽量不要混用 `new` 和智能指针（如 `shared_ptr<T> p(new T())`），因为这不仅慢（两次内存分配），而且如果 `new` 成功但构造智能指针失败，会发生泄漏。请坚持使用 `make_unique` 和 `make_shared`。
3. **理解 Move**：理解 `std::move` 是理解 `unique_ptr` 的关键。它并不“移动”数据，只是将指针的所有权“窃取”过来，并将原指针置空。