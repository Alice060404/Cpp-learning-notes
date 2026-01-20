/**
 * @file arrays_deep_dive.cpp
 * @brief 深入演示 C++ 数组：内存模型、指针运算、堆分配及现代替代方案
 */

#include <iostream>
#include <array>  // C++11 std::array
#include <vector> // 动态数组的首选

// 演示数组退化 (Pointer Decay)
// 这里的 arr 虽然写着 int arr[]，但实际上它只是一个 int* 指针
void printArraySize(int arr[], size_t explicitSize)
{
    // ? 错误做法：在这里使用 sizeof(arr)
    // 在 64 位系统上，这里通常输出 8 (指针大小)，而不是数组总字节数
    std::cout << "[Function] sizeof(arr) inside function: " << sizeof(arr) << " bytes (Pointer size!)" << std::endl;

    // ? 正确做法：依赖传递进来的 explicitSize
    std::cout << "[Function] Printing array using explicit size: ";
    for (size_t i = 0; i < explicitSize; ++i)
    {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main()
{
    // ==========================================
    // 1. 栈数组与指针运算 (Stack Array & Pointers)
    // ==========================================
    std::cout << "=== 1. Stack Array & Pointer Arithmetic ===" << std::endl;
    int stackArr[4] = {10, 20, 30, 40};

    // 证明：数组名即指针
    std::cout << "Address of stackArr:    " << stackArr << std::endl;
    std::cout << "Address of stackArr[0]: " << &stackArr[0] << std::endl;

    // 证明：下标访问只是指针运算的语法糖
    // 访问第 3 个元素 (Index 2)
    std::cout << "Access via Index stackArr[2]:   " << stackArr[2] << std::endl;
    std::cout << "Access via Pointer *(stackArr+2): " << *(stackArr + 2) << std::endl;

    // 计算大小技巧 (仅在数组定义的作用域内有效)
    size_t length = sizeof(stackArr) / sizeof(stackArr[0]);
    std::cout << "Array Length: " << length << "\n"
              << std::endl;

    // ==========================================
    // 2. 数组退化陷阱 (The Decay Pitfall)
    // ==========================================
    std::cout << "=== 2. Pointer Decay Demo ===" << std::endl;
    std::cout << "[Main] sizeof(stackArr) in main: " << sizeof(stackArr) << " bytes (Full size)" << std::endl;
    printArraySize(stackArr, length); // 传递数组

    // ==========================================
    // 3. 堆数组分配 (Heap Allocation)
    // ==========================================
    std::cout << "\n=== 3. Heap Allocation (Manual Memory) ===" << std::endl;
    size_t dynamicSize;
    std::cout << "Enter size for dynamic array: ";
    // std::cin >> dynamicSize; // 为了演示方便，这里硬编码
    dynamicSize = 5;
    std::cout << dynamicSize << std::endl;

    // 步骤 A: 分配 (使用 new[])
    int *heapArr = new int[dynamicSize];

    // 步骤 B: 初始化与使用
    for (size_t i = 0; i < dynamicSize; ++i)
    {
        heapArr[i] = (i + 1) * 111;
        std::cout << heapArr[i] << " ";
    }
    std::cout << std::endl;

    // 步骤 C: 释放 (非常重要！必须使用 delete[])
    delete[] heapArr;
    heapArr = nullptr; // 最佳实践：释放后置空，防止悬空指针
    std::cout << "Heap memory freed." << std::endl;

    // ==========================================
    // 4. 现代 C++ 方案 (Modern Alternatives)
    // ==========================================
    std::cout << "\n=== 4. Modern C++: std::array & std::vector ===" << std::endl;

    // std::array (固定大小，栈分配，更安全)
    std::array<int, 3> safeArr = {1, 2, 3};
    std::cout << "std::array size: " << safeArr.size() << std::endl; // 自带 size()

    // 范围 for 循环 (Range-based for loop) - 现代 C++ 遍历神器
    std::cout << "Traversing std::array: ";
    for (const auto &val : safeArr)
    {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // std::vector (动态大小，堆分配，自动管理内存) - 推荐替代 new[]
    std::vector<int> dynamicVec;
    dynamicVec.push_back(100);
    dynamicVec.push_back(200);
    std::cout << "std::vector size: " << dynamicVec.size() << " (No manual delete needed!)" << std::endl;

    return 0;
}