[MindMap](./String_Literals.png)

[demo](./demo.cpp)


# C++ 核心机制：字符串字面量 (String Literals)

> **定义**：代码中用双引号括起来的字符序列（例如 `"Hello World"`）。
> **核心本质**：它们不仅仅是文本，它们是**存储在静态只读内存区**的 `const char` 数组。

## 1. 内存模型与生命周期 (Memory & Lifecycle)
理解字符串字面量存放的位置是防止程序崩溃的关键。

### A. 存储位置：只读数据段 (.rodata)
* **静态存储期**：字符串字面量在程序启动时就存在，直到程序结束才销毁。
* **共享机制**：现代编译器会进行“字符串驻留” (String Interning)。如果代码中出现多次 `"Hello"`，编译器可能只在内存中存一份，所有指针都指向同一个地址。
* **只读保护**：尝试修改字符串字面量会导致 **未定义行为 (Undefined Behavior)**，通常表现为段错误 (Segmentation Fault) 或 访问冲突 (Access Violation)。

### B. 两种定义的巨大差异
这是新手最容易混淆的地方：

1.  **指针方式 (危险区)**
    ```cpp
    const char* ptr = "Hello"; 
    ```
    * `ptr` 指向只读内存区。
    * **绝对不可修改** `ptr[0] = 'X'` (会崩溃)。

2.  **数组方式 (安全区)**
    ```cpp
    char arr[] = "Hello";
    ```
    * 编译器会将 "Hello" 从只读内存**拷贝**一份到栈 (Stack) 上。
    * `arr` 是栈上的独立副本，**可以任意修改**。

## 2. 原始字符串字面量 (Raw String Literals) - C++11
* **痛点**：在普通字符串中，反斜杠 `\` 是转义符。如果你要写 Windows 路径或正则表达式，必须写成 `\\`。
    * 例如：`"C:\\Windows\\System32"` (非常难读)。
* **解决方案**：使用 `R"(...)"` 语法。
    * 括号内的所有字符（包括换行、反斜杠）都视为普通字符，**不进行转义**。
    * 示例：`R"(C:\Windows\System32)"`。
    * **场景**：正则表达式 (Regex)、JSON 字符串、SQL 查询语句、多行文本。

## 3. 前缀与字符类型 (Prefixes & Encodings)
随着 C++ 对国际化的支持，字符串有了更多的前缀来指定编码：

| 前缀 | 类型 | 含义 | 典型用途 |
| :--- | :--- | :--- | :--- |
| 无 | `const char[N]` | 本地编码 (通常 ASCII/UTF-8) | 传统代码 |
| **`L`** | `const wchar_t[N]` | 宽字符 | Windows API 调用 |
| **`u8`** | `const char[N]` (C++11) | **强制 UTF-8 编码** | 跨平台网络传输、JSON |
| **`u`** | `const char16_t[N]` | UTF-16 | ICU 库、Java 交互 |
| **`U`** | `const char32_t[N]` | UTF-32 | 完整 Unicode 处理 |

## 4. 特殊后缀与拼接 (Suffixes & Concatenation)

### A. `s` 后缀 (C++14)
* 通常 `"Hello"` 的类型是 `const char*`。
* 使用 `"Hello"s` (需 `using namespace std::string_literals;`)，其类型直接变为 **`std::string` 对象**。
* *优势*：可以直接调用 `.size()` 或 `+` 拼接，无需手动构造。

### B. 编译期自动拼接
* C++ 允许将多个连续的字符串字面量（中间用空格、换行分隔）视为一个字符串。
* 写法：`"Hello " "World"` 等价于 `"Hello World"`。
* *场景*：编写超长的 SQL 语句或日志信息时换行美观。

---