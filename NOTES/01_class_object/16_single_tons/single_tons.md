[MindMap](./single_tons.png)


# C++ 设计模式笔记：单例模式 (Singleton Pattern)

## 1. 核心定义与设计理念

单例模式（Singleton Pattern）是一种创建型设计模式，其核心目的是**保证一个类仅有一个实例，并提供一个访问它的全局访问点**。

对于计算机专业的学生来说，可以将其理解为一种**受控的全局变量**。在操作系统或大型系统中，有些资源在同一时刻只能有一个管理者，例如：

* **显卡驱动程序**：不能有两个驱动同时指挥显卡渲染。
* **文件系统**：需要统一管理文件的读写锁。

### 为什么不直接用全局变量？

虽然全局变量也能实现“全局访问”，但单例模式解决了全局变量的两个致命缺陷：

1. **缺乏组织性**：全局变量散落在代码各处，污染全局命名空间，维护困难。
2. **初始化时机不可控**：C++中不同编译单元（.cpp文件）的全局变量初始化顺序是未定义的（Initialization Order Fiasco）。如果一个全局变量依赖另一个全局变量，可能会导致程序崩溃。而单例模式可以利用“延迟实例化”（Lazy Initialization）解决这个问题。

## 2. 核心实现步骤

要实现一个标准的单例类，必须在C++类设计中严格遵守以下规则（对应思维导图中的实现步骤）：

### 2.1 构造函数私有化 (Private Constructor)

* **目的**：禁止在类外部通过 `new` 或直接定义变量的方式创建对象。
* **操作**：将默认构造函数声明为 `private`。

### 2.2 静态访问方法 (Static Access Method)

* **目的**：提供唯一的外部访问接口。
* **命名习惯**：通常命名为 `GetInstance()` 或 `Get()`。
* **返回值**：通常返回该类唯一实例的**引用**（`Type&`）或指针。建议返回引用，因为引用不能为 `nullptr`，语义更清晰且安全。

### 2.3 禁止拷贝与赋值 (Disable Copy/Assignment)

* **目的**：确保实例的唯一性。如果不禁止，用户可能会通过拷贝构造函数复制出一个新的“单例”，破坏模式约定。
* **操作**：在C++11及以后，使用 `= delete` 关键字显式删除拷贝构造函数和拷贝赋值运算符。

## 3. 优化技术：Meyers' Singleton (现代C++首选)

思维导图中提到的“局部静态变量”和“延迟实例化”在现代C++中有一个完美的实现方案，被称为 **Meyers' Singleton**（由《Effective C++》作者 Scott Meyers 提出）。

### 局部静态变量的魔法

在C++11标准之前，在多线程环境下初始化静态局部变量是不安全的。但**C++11标准规定：如果指令逻辑进入一个未被初始化的声明变量，所有并发执行应当等待该变量完成初始化。**

这意味着：

1. **线程安全**：不需要额外的锁（Mutex）就能保证并发安全。
2. **延迟实例化**：只有当第一次调用 `Get()` 函数时，对象才会被创建，节省资源。
3. **代码简洁**：极大地简化了传统的“双重检查锁定”（Double-Checked Locking）写法。

## 4. 应用场景示例

* **日志系统 (Logger)**：整个程序应该向同一个日志文件写入，避免多文件句柄冲突。
* **配置管理器 (ConfigManager)**：程序启动时读取一次配置文件，之后全局共享这份配置数据。
* **随机数生成器**：维护一个全局的随机数引擎状态。

## 5. 方案对比

| 特性 | 单例模式 (Singleton) | 命名空间 (Namespace) | 全局变量 |
| --- | --- | --- | --- |
| **对象特性** | 支持（可以作为参数传递、继承等） | 不支持（仅是作用域） | 支持 |
| **初始化时机** | 可控（首次调用时/延迟加载） | 编译期/程序启动时 | 难以控制（顺序不确定） |
| **封装性** | 高（可以控制访问权限） | 低（成员全部公开） | 低 |
| **主要缺点** | 隐藏了类之间的依赖关系 | 无法实例化 | 污染命名空间 |

---

## 附录：C++ 代码实现

以下代码展示了一个基于 **Meyers' Singleton** 的完整实现。你可以直接将其作为模板使用。

**文件名**: `SingletonDemo.cpp`

```cpp
#include <iostream>
#include <string>
#include <vector>

// 这是一个典型的单例类：日志系统
class Logger {
public:
    // 1. 获取唯一实例的静态方法 (全局访问点)
    // 使用局部静态变量 (Magic Static) 实现，C++11起线程安全
    static Logger& Get() {
        static Logger instance; // 唯一实例，仅在首次调用 Get() 时初始化
        return instance;
    }

    // 2. 删除拷贝构造函数和赋值运算符 (禁止拷贝控制)
    // 这样做可以防止代码中出现 Logger log2 = Logger::Get(); 这样的逻辑，确保实例唯一
    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;

    // 业务功能：写日志
    void Log(const std::string& message) {
        // 在实际项目中，这里可能会加锁 (std::mutex) 以保证多线程写入文件不混乱
        // 这里仅作演示
        logs_.push_back(message);
        std::cout << "[LOG]: " << message << std::endl;
    }

    // 业务功能：查看历史日志数量
    size_t GetLogCount() const {
        return logs_.size();
    }

private:
    // 3. 构造函数私有化 (防止外部实例化)
    Logger() {
        std::cout << "Logger system initialized." << std::endl;
    }

    // 析构函数也设为私有或保持默认，但在单例中通常由系统在程序结束时自动调用
    ~Logger() {
        std::cout << "Logger system shut down." << std::endl;
    }

    std::vector<std::string> logs_;
};

// 模拟一个使用日志的模块
void FunctionA() {
    // 并不需要传递 Logger 对象，直接通过 Get() 访问
    Logger::Get().Log("Function A executed.");
}

// 模拟另一个使用日志的模块
void FunctionB() {
    Logger::Get().Log("Function B executed.");
}

int main() {
    std::cout << "--- Program Start ---" << std::endl;

    // 第一次调用 Get()，此时会触发 Logger 的构造函数
    Logger::Get().Log("Main function started.");

    FunctionA();
    FunctionB();

    // 验证是否是同一个实例
    std::cout << "Total logs recorded: " << Logger::Get().GetLogCount() << std::endl;
    
    // 下面的代码如果取消注释会报错，因为构造函数是私有的且禁止了拷贝
    // Logger l; // Error
    // Logger l2 = Logger::Get(); // Error

    std::cout << "--- Program End ---" << std::endl;
    return 0;
}

```