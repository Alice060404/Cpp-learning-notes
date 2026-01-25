[MindMap](./Benchmarking.png)


#  C++ 性能基准测试笔记 (Benchmarking)

> **摘要**：本笔记整理自性能测试思维导图。主要记录如何正确地测量 C++ 代码执行时间，如何利用 RAII 机制实现自动化计时器，以及如何避开编译器优化带来的测试陷阱。

## 1. 核心概念：什么是基准测试？

基准测试（Benchmarking）是通过运行一段代码并测量其执行时间、内存占用等指标，来评估程序性能的过程。

* **没有标准答案**：性能取决于硬件、操作系统、编译器版本甚至当前 CPU 的温度。我们追求的通常是**相对性能**（方案 A 比方案 B 快多少）。
* **不仅仅是计时**：除了时间，还涉及缓存命中率 (Cache Misses)、内存分配次数等。但在初学阶段，关注**执行时间 (Wall-clock time)** 最为直接。

## 2. 工具与方法

### 2.1 计时手段

* **手动计时器**：在代码段前后分别获取时间戳，相减得到时长。
* **RAII 计时器 (Scope-based Timer)**：利用 C++ 的构造函数和析构函数特性，实现“进入作用域开始计时，离开作用域自动停止并输出”。这是 C++ 特有的优雅写法。
* **外部工具**：如 Linux 的 `time` 命令，或者专业的 Profiler（Visual Studio Profiler, Valgrind, Intel VTune）。

### 2.2 现代 C++ 时钟 (`std::chrono`)

老旧的 `clock()` 或 `time()` 精度不够（通常只有毫秒级），且受系统时间调整影响。现代 C++ (C++11 及以后) 标准推荐使用 `<chrono>` 库：

* `std::chrono::high_resolution_clock`：提供当前硬件能支持的最高精度。
* `std::chrono::microseconds` / `nanoseconds`：微秒/纳秒级单位转换。

## 3. 编译器优化的陷阱 (The Optimization Trap)

这是初学者做测试最容易翻车的地方。**Debug 模式和 Release 模式的性能差异可能是 10 倍甚至 100 倍。**

### 3.1 死代码消除 (Dead Code Elimination)

如果你写了一个循环计算 `a + b`，但在循环结束后没有使用这个计算结果，编译器在 Release 模式下会认为这段代码是“无用的（Dead Code）”，直接把整个循环删掉。

* **现象**：测试结果显示运行时间为 0ns。
* **对策**：确保计算结果被输出到控制台，或者使用 `volatile` 关键字强制读取内存。

### 3.2 常量折叠 (Constant Folding)

如果你的输入都是常量（例如 `add(3, 4)`），编译器会在编译阶段直接算出 `7`，运行时根本不会执行加法指令。

* **对策**：使用运行时生成的随机数或用户输入作为测试数据。

## 4. 实战案例：智能指针性能对比

一个经典的测试案例是对比 `std::shared_ptr` 的两种初始化方式：

1. `std::shared_ptr<T>(new T)`：先 `new` 对象，再创建智能指针（**两次** 内存分配）。
2. `std::make_shared<T>()`：同时分配对象和控制块（**一次** 内存分配）。
理论上 `make_shared` 更快，我们将通过代码验证这一点。

---

##  5. C++ 代码示例：RAII 计时器与实战

以下代码实现了一个基于 RAII 的 `Timer` 类，并用它来对比智能指针的创建性能。

> **文件名**: `benchmark_demo.cpp`
> **编译建议**: `g++ -O3 benchmark_demo.cpp -o benchmark_demo` (务必开启 -O2 或 -O3 优化)

```cpp
/**
 * @file benchmark_demo.cpp
 * @brief C++ 性能基准测试示例：RAII 计时器与智能指针性能对比
 */

#include <iostream>
#include <chrono>
#include <memory>  // for std::shared_ptr, std::make_shared
#include <vector>

// ==========================================
// 1. RAII 计时器类 (Scope-based Timer)
// ==========================================
class Timer {
public:
    // 构造函数：对象创建时记录开始时间
    Timer(const char* funcName) : m_Name(funcName), m_Stopped(false) {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    // 析构函数：对象销毁时（离开作用域）自动停止计时并打印
    ~Timer() {
        Stop();
    }

    void Stop() {
        if (m_Stopped) return;

        auto endTime = std::chrono::high_resolution_clock::now();
        
        // 转换时间差为微秒 (microseconds)
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime).time_since_epoch().count();

        long long duration = end - start;
        double ms = duration * 0.001; // 转换为毫秒

        std::cout << "[" << m_Name << "] Duration: " << duration << "us (" << ms << "ms)" << std::endl;
        m_Stopped = true;
    }

private:
    const char* m_Name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    bool m_Stopped;
};

// ==========================================
// 2. 测试对象
// ==========================================
struct Vector3 {
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
};

// ==========================================
// 3. 测试函数
// ==========================================

void TestNewShared() {
    // 只要在这个作用域内，Timer 就在计时
    Timer timer("New Shared Ptr");
    
    for (int i = 0; i < 1000000; ++i) {
        // 方式 1: 显式 new (性能较低，两次分配)
        std::shared_ptr<Vector3> ptr(new Vector3());
    }
}

void TestMakeShared() {
    Timer timer("Make Shared");

    for (int i = 0; i < 1000000; ++i) {
        // 方式 2: make_shared (性能较高，一次分配)
        std::shared_ptr<Vector3> ptr = std::make_shared<Vector3>();
    }
}

int main() {
    std::cout << "Starting Benchmark (Run in Release Mode! -O3)..." << std::endl;

    // 运行多次以观察稳定性
    TestNewShared();
    TestMakeShared();
    
    std::cout << "--------------------------------" << std::endl;

    TestNewShared();
    TestMakeShared();

    return 0;
}

```

###  代码解析与拓展

1. **RAII 机制**：注意 `Timer` 类没有显式的 `start()` 或 `end()` 调用。当你写 `Timer timer("Name");` 时，计时开始；当函数 `TestMakeShared` 结束，`timer` 变量超出作用域被销毁，析构函数自动触发，打印时间。这是 C++ 管理资源（内存、文件句柄、时间）最核心的哲学。
2. **单位转换**：代码中使用了 `time_point_cast` 将高精度时间转换为微秒 (`long long`)，便于阅读。
3. **结果预期**：在开启优化（`-O3`）的情况下，`Make Shared` 的耗时通常会明显少于 `New Shared Ptr`，因为减少了 50% 的堆内存分配次数（Heap Allocation），且内存局部性更好。