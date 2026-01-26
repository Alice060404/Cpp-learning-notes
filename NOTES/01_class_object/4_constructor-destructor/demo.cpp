/**
 * @file lifecycle_demo.cpp
 * @brief 演示 C++ 构造函数与析构函数的生命周期管理
 * @note 重点观察控制台输出的顺序
 */

#include <iostream>
#include <string>

class InventoryItem
{
private:
    std::string name;
    int *quantityPtr; // 使用指针来演示动态内存分配和清理

public:
    // ==========================================
    // 1. 默认构造函数 (Default Constructor)
    // ==========================================
    InventoryItem()
    {
        name = "Unknown";
        quantityPtr = new int(0); // 防止随机值，分配堆内存
        std::cout << "[构造] Default created: " << name << std::endl;
    }

    // ==========================================
    // 2. 带参数构造函数 (Parameterized Constructor)
    // ==========================================
    InventoryItem(std::string n, int q)
    {
        name = n;
        quantityPtr = new int(q); // 分配内存/设置状态
        std::cout << "[构造] Param created: " << name << " with quantity " << *quantityPtr << std::endl;
    }

    // ==========================================
    // 3. 析构函数 (Destructor)
    // ==========================================
    // 特征：~开头，无参数，负责 Cleanup
    ~InventoryItem()
    {
        std::cout << "[析构] Destroying: " << name << std::endl;

        // 关键用途：防止内存泄漏
        if (quantityPtr != nullptr)
        {
            delete quantityPtr;
            quantityPtr = nullptr;
            std::cout << "       -> Memory cleaned up." << std::endl;
        }
    }

    // 获取名称用于演示
    std::string getName() const { return name; }
};

int main()
{
    std::cout << "=== Main Function Started ===" << std::endl;

    // 场景 A: 栈分配 (Stack Allocation)
    // 作用域：仅在大括号 {} 内有效
    {
        std::cout << "\n--- Entering Scope ---" << std::endl;
        InventoryItem item1("Apple", 10); // 调用带参构造

        // item1 在这里是存活的
    } // <--- 作用域结束，item1 自动触发析构函数 (超出作用域)
    std::cout << "--- Exited Scope (item1 should be gone) ---" << std::endl;

    // 场景 B: 堆分配 (Heap Allocation)
    // 需要显式管理生命周期
    std::cout << "\n--- Heap Allocation ---" << std::endl;
    InventoryItem *item2 = new InventoryItem("Golden Sword", 1); // 调用带参构造

    // 堆对象不会自动销毁，即使 main 函数结束也不会（除非操作系统回收）
    // 必须手动调用 delete
    std::cout << "--- Manually Deleting item2 ---" << std::endl;
    delete item2; // <--- 显式触发析构函数

    std::cout << "\n=== Main Function Ended ===" << std::endl;
    return 0;
}