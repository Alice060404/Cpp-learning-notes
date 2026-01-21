[MindMap](./create_object.png)


# C++ 对象实例化与内存模型 (Object Instantiation & Memory Model)

## 1. 核心概念：内存的双城记 (Stack vs. Heap)

在 C++ 中，对象在哪里创建决定了它的**生命周期**、**访问速度**以及**管理方式**。

### 1.1 栈 (Stack) —— "秩序与速度"

* **分配机制**：栈内存由 CPU 指令直接控制（移动栈指针 `ESP/RSP`），分配和释放仅仅是指令指针的移动。
* **特点**：
* **极速**：遵循 LIFO（后进先出），内存连续，缓存命中率高。
* **自动管理 (RAII)**：这是 C++ 的核心优势。一旦变量离开作用域（Scope，即当前的大括号 `{}`），析构函数会被自动调用，内存自动回收。
* **空间有限**：通常只有 1MB - 2MB（Windows 默认 1MB，Linux 默认 8MB）。如果创建超大数组或发生无限递归，会导致 **Stack Overflow**。


* **适用场景**：局部变量、函数参数、生命周期短且确定的小对象。

### 1.2 堆 (Heap) —— "自由与代价"

* **分配机制**：堆是操作系统管理的一大块空闲内存链表。分配时需要查找足够大的空闲块，可能引发用户态到内核态的切换。
* **特点**：
* **相对较慢**：存在分配开销（Allocation Overhead）和内存碎片（Fragmentation）问题。
* **手动管理**：程序员掌握生杀大权。必须手动申请 (`new`) 且手动释放 (`delete`)。
* **空间巨大**：受限于物理内存和虚拟内存空间（GB 级别）。


* **适用场景**：对象需要跨函数生存、对象体积极大（如加载一张 4K 图片）、对象数量在运行时才能确定。

## 2. 实例化方式对比

### 2.1 在栈上创建 (Stack Instantiation) —— **首选方式**

这是 C++ 与 Java/Python 最大的不同点。在 C++ 中，默认应该在栈上创建对象。

* **语法**：
```cpp
ClassName obj;          // 调用默认构造
ClassName obj(10, "A"); // 调用带参构造

```


* **优势**：
* **零内存泄漏风险**：即使发生异常，栈展开（Stack Unwinding）也会保证析构函数被执行。
* **语法简洁**：使用 `.` 操作符访问成员。



### 2.2 在堆上创建 (Heap Instantiation)

* **语法**：
```cpp
ClassName* ptr = new ClassName(); // 返回指针
// ... 使用 ...
delete ptr;                       // 必须手动释放！

```


* **劣势与风险**：
* **内存泄漏 (Memory Leak)**：如果忘记 `delete`，或者在 `delete` 之前函数提前 `return` 或抛出异常，这块内存将永远无法回收（直到进程结束）。
* **访问繁琐**：必须使用箭头操作符 `->` 访问成员，或者解引用 `*ptr`。



## 3. 进阶：现代 C++ 的解决方案 (Smart Pointers)

思维导图中提到了“进阶概念”。作为 CS 学生，现在写 C++ **强烈不建议**使用裸指针 (`new/delete`)，而应使用 **RAII (资源获取即初始化)** 机制封装的智能指针。

* **`std::unique_ptr`**：独占所有权，超出作用域自动 `delete`（栈的自动管理特性 + 堆的大空间特性）。
* **`std::shared_ptr`**：引用计数，多个指针共享同一个对象。

---

## 4. 代码实战示例

```cpp
/*
 * 文件名: memory_instantiation.cpp
 * 描述: 演示栈与堆的对象实例化、生命周期差异及智能指针的使用
 * 作者: Computer Science Freshman
 * 编译指令: g++ -o memory_test memory_instantiation.cpp -std=c++14
 */

#include <iostream>
#include <string>
#include <memory> // 用于智能指针

using namespace std;

// 定义一个简单的类，通过打印日志来追踪生命周期
class Entity {
private:
    string m_Name;
public:
    Entity() : m_Name("Unknown") {
        cout << "[Entity Created] Default Constructor: " << m_Name << endl;
    }
    
    Entity(const string& name) : m_Name(name) {
        cout << "[Entity Created] Param Constructor: " << m_Name << endl;
    }

    ~Entity() {
        cout << "[Entity Destroyed] Destructor: " << m_Name << endl;
    }

    void PrintInfo() const {
        cout << "    -> Processing: " << m_Name << endl;
    }
};

// ==========================================
// 1. 栈上实例化 (Stack) - 推荐
// ==========================================
void CreateOnStack() {
    cout << "\n--- Function: CreateOnStack Start ---" << endl;
    
    // 语法：类名 变量名(参数);
    // 内存：分配在栈帧(Stack Frame)上
    Entity stackEntity("StackObject"); 
    stackEntity.PrintInfo();

    // 关键点：函数结束（遇到右大括号），stackEntity 自动销毁
    cout << "--- Function: CreateOnStack End (Auto Destroy follows) ---" << endl;
}

// ==========================================
// 2. 堆上实例化 (Heap) - 传统方式 (不推荐)
// ==========================================
void CreateOnHeap() {
    cout << "\n--- Function: CreateOnHeap Start ---" << endl;

    // 语法：使用 new 关键字
    // 内存：在堆内存寻找空闲块，返回地址给栈上的指针变量
    Entity* heapPtr = new Entity("HeapObject_Raw");

    // 访问：必须使用箭头 ->
    heapPtr->PrintInfo();

    // 风险演示：如果这里忘记写 delete，就会发生内存泄漏！
    // 析构函数永远不会被调用。
    
    delete heapPtr; // 手动释放
    // heapPtr 此时变成悬空指针 (Dangling Pointer)，建议置空
    heapPtr = nullptr; 

    cout << "--- Function: CreateOnHeap End ---" << endl;
}

// ==========================================
// 3. 堆上实例化 (Heap) - 现代方式 (智能指针)
// ==========================================
void CreateWithSmartPointer() {
    cout << "\n--- Function: CreateWithSmartPointer Start ---" << endl;

    // unique_ptr: 独占这个对象，本身是一个栈对象，但在内部管理堆内存
    // make_unique 是 C++14 标准，最安全的创建方式
    unique_ptr<Entity> smartPtr = make_unique<Entity>("SmartHeapObject");

    // 使用方式和普通指针类似
    smartPtr->PrintInfo();

    // 优势：不需要写 delete！
    // 当 smartPtr 离开作用域时，它会自动调用 delete 释放内部的堆内存
    cout << "--- Function: CreateWithSmartPointer End (Auto Destroy follows) ---" << endl;
}

int main() {
    // 1. 栈对象演示
    CreateOnStack();

    // 2. 堆对象演示 (手动管理)
    CreateOnHeap();

    // 3. 智能指针演示 (自动管理堆内存)
    CreateWithSmartPointer();

    // 4. 作用域演示
    cout << "\n--- Scope Demo ---" << endl;
    {
        Entity scopedObj("ScopedEntity");
        cout << "    (Inside Scope Block)" << endl;
    } // scopedObj 在这里立即销毁，而不是等 main 函数结束
    cout << "--- After Scope Block ---" << endl;

    return 0;
}

```

### 代码运行结果预期

观察输出，你会发现：

1. **栈对象**：在 `CreateOnStack` 函数结束前自动打印 "Destroyed"。
2. **堆对象**：必须显式调用 `delete` 才会打印 "Destroyed"。
3. **作用域演示**：`ScopedEntity` 在大括号 `}` 处立即销毁，验证了栈对象的生命周期严格绑定于作用域。