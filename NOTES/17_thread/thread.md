[MindMap](./thread.png)


# C++ 多线程编程入门详解 (Multithreading with std::thread)

## 1. 核心概念：打破“顺序执行”的枷锁

在单线程程序中，代码是一行一行执行的。如果第 5 行在等待用户输入 (`std::cin`)，第 6 行的代码就必须干等着，CPU 处于空闲（Blocked）状态。

* **多线程 (Multithreading)**：允许程序同时运行多个“执行流”。
* **物理基础**：现代 CPU 都是多核的（Multi-core）。多线程允许我们将任务分配给不同的核心，实现真正的**并行计算 (Parallelism)**。
* **逻辑线程**：即使在单核 CPU 上，操作系统也会通过极其快速的**上下文切换 (Context Switching)**，让你感觉多个任务在同时运行（比如一边下载文件，一边响应鼠标点击）。

## 2. 核心 API 解析 (`<thread>`)

### 2.1 `std::thread` —— 线程的大脑

这是 C++11 引入的线程管理类。

* **启动线程**：`std::thread worker(FunctionPtr);`
* 一旦这行代码执行，新的线程**立即启动**，操作系统开始调度它。
* 它可以接受函数指针、Lambda 表达式或仿函数。


* **传参**：`std::thread worker(FunctionPtr, arg1, arg2);`
* *注意*：默认情况下参数是**拷贝**传递的。如果需要引用传递，必须使用 `std::ref(arg)`。



### 2.2 线程生命周期：`join()` vs `detach()`

这是新手最容易导致程序崩溃 (`std::terminate`) 的地方。当 `std::thread` 对象销毁时（例如离开作用域），它必须处于“明确状态”。

* **`join()` (汇合/等待)**：
* **含义**：主线程（Main Thread）卡在这里，直到子线程执行完毕。
* **用途**：确保子线程在主线程退出前完成工作，防止访问已销毁的资源。


* **`detach()` (分离/放飞)**：
* **含义**：切断主线程与子线程的联系。子线程在后台独立运行，主线程不再管它。
* **风险**：如果主程序结束了，分离的线程会被操作系统强行杀死，可能导致文件未保存等问题。**新手建议只用 `join()**`。



### 2.3 `std::this_thread` —— 当前线程的操作

* **`get_id()`**：获取当前线程的唯一 ID，用于调试。
* **`sleep_for()`**：让当前线程“休眠”指定时间，让出 CPU 时间片给其他线程。这是降低 CPU 占用率的常用手段。

## 3. 关键陷阱：数据竞争 (Data Race)

思维导图中提到了“布尔标志位同步”。这里必须警惕：

* **问题**：当多个线程同时修改同一个变量（如 `bool isFinished = false`），会导致未定义行为。
* **解决**：虽然大一阶段可能只需了解概念，但在工程中应使用 `std::atomic<bool>` 或 `std::mutex`（互斥锁）来保护共享数据。

---

## 4. 代码实战笔记

```cpp
/*
 * 文件名: multithreading_demo.cpp
 * 描述: 演示 C++ 多线程的创建、并行执行、阻塞等待及 CPU 优化技巧
 * 编译: g++ -o threads multithreading_demo.cpp -std=c++14 -pthread
 * 注意: Linux/WSL 环境下通常需要加 -pthread 链接库
 */

#include <iostream>
#include <thread>   // 核心头文件
#include <chrono>   // 时间库，用于 sleep
#include <string>

using namespace std;

// 这是一个将在后台运行的“工作函数”
// 模拟一个耗时的任务，比如下载文件或加载游戏资源
void WorkerFunction(int id, bool& stopFlag) {
    cout << "[Worker] Thread started. ID: " << std::this_thread::get_id() << endl;

    while (!stopFlag) {
        cout << "[Worker] Doing generic work... (ID: " << id << ")" << endl;
        
        // 模拟耗时操作，同时让出 CPU，避免死循环占用 100% CPU
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << "[Worker] Received stop signal. Cleaning up..." << endl;
}

int main() {
    cout << "=== Multithreading Demo ===" << endl;
    cout << "[Main] Application started. Main Thread ID: " << std::this_thread::get_id() << endl;

    bool stopSignal = false; // 用于控制子线程何时结束

    // 1. 创建并启动线程
    // 语法: std::thread 变量名(函数名, 参数1, 参数2...)
    // 注意: 这里传递 stopSignal 的引用必须用 std::ref，否则传进去的是拷贝，子线程永远读不到 true
    std::thread worker(WorkerFunction, 999, std::ref(stopSignal));

    // 此时，WorkerFunction 已经在另一个 CPU 核心上跑起来了
    // 主线程继续往下执行，不会被阻塞

    // 2. 主线程处理自己的逻辑
    // 比如：监听用户输入，或者渲染 UI
    cout << "[Main] Press ENTER to stop the worker thread..." << endl;
    cin.get(); // 阻塞操作：等待用户按下回车

    // 3. 线程间通信与同步
    cout << "[Main] Setting stop flag to true..." << endl;
    stopSignal = true;

    // 4. 等待线程结束 (Join)
    // 这一步至关重要！如果我们不 join，main 函数结束会导致 worker 对象析构，
    // 而此时线程还在运行，C++ 运行时会直接调用 std::terminate() 导致崩溃。
    if (worker.joinable()) {
        worker.join(); // 主线程在此暂停，直到 worker 线程函数 return
        cout << "[Main] Worker thread joined successfully." << endl;
    }

    cout << "[Main] Exiting program." << endl;
    return 0;
}

```

### 学习要点 (Key Takeaways)

1. **并行思维**：运行这段代码时，你会发现 `[Worker]` 的打印和 `[Main]` 的等待是同时发生的。这就是多线程的魔力。
2. **必须 Join**：养成好习惯，创建了线程就要负责回收它。在 `main` 退出前务必调用 `join()`。
3. **引用传递**：`std::thread` 的构造函数对参数进行的是**值拷贝**。如果你想让线程修改外部变量（如本例的 `stopFlag`），必须显式使用 `std::ref()`。
4. **编译参数**：如果你在 WSL (Ubuntu) 下编译，报错 `undefined reference to pthread_create`，请务必在编译指令最后加上 `-pthread`。