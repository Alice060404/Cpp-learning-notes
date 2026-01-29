# C++ Learning Notes

## 简介 (Introduction)

仓库中的内容不仅包含了基础语法和面向对象编程（OOP）的核心概念，还涵盖了现代 C++ 特性（如智能指针、移动语义、Lambda 表达式）以及常用的算法与数据结构实现。


## 目录导航 (Navigation)

### 0. 算法与数据结构 (Algorithms)
这里包含了一些基础算法的 C++ 实现与分析。
* [高精度加法 (High-precision Adder)](./NOTES/00_algo/High-precision_Adder/README.md)
* [Kadane 算法 (最大子数组和)](./NOTES/00_algo/Kadane_algorithm/README.md)
* [二进制加法实现](./NOTES/00_algo/binary_add/)
* [标准排序 std::sort 解析](./NOTES/00_algo/Sort/std_sort/std_sort.md)

### 1. 面向对象编程 (Object-Oriented Programming)
C++ 的核心特性，包含类、对象、继承与多态的详细笔记。
* **基础概念**
    * [类 (Class) 的基本定义](./NOTES/01_class_object/1_class/README.md)
    * [Class 与 Struct 的区别](./NOTES/01_class_object/2_class-vs-struct/README.md)
    * [如何编写一个类](./NOTES/01_class_object/3_write_class/README.md)
    * [构造函数与析构函数](./NOTES/01_class_object/4_constructor-destructor/README.md)
    * [对象创建方式](./NOTES/01_class_object/12_create_object/create_object.md)
* **封装与访问控制**
    * [可见性与封装 (Visibility)](./NOTES/01_class_object/9_Visibility_and_Encapsulation/README.md)
    * [Static 关键字](./NOTES/01_class_object/5_static/README.md)
    * [友元 (Friend)](./NOTES/01_class_object/7_friend/README.md)
    * [This 指针](./NOTES/01_class_object/14_this/this.md)
* **深入对象模型**
    * [深拷贝与浅拷贝](./NOTES/01_class_object/10_shallow_deep_copy/shallow_deep_copy.md)
    * [初始化列表](./NOTES/01_class_object/11_initialization_list/initialization_list.md)
    * [隐式转换与 explicit 关键字](./NOTES/01_class_object/13_Implicit_explicit/Implicit_explicit.md)
    * [单例模式 (Singleton)](./NOTES/01_class_object/16_single_tons/single_tons.md)
* **继承与多态**
    * [继承与虚函数](./NOTES/01_class_object/8_inheritance_poly/inheritance/README.md)
    * [虚析构函数](./NOTES/01_class_object/8_inheritance_poly/virtual_destructor/virtual_destructor.md)
    * [Dynamic Cast](./NOTES/01_class_object/15_dynamic_cast/dynamic_cast.md)
    * [枚举 (Enums)](./NOTES/01_class_object/6_enums/README.md)

### 2. 内存管理 (Memory Management)
理解 C++ 的内存模型是进阶的关键。
* [Stack vs Heap (栈与堆)](./NOTES/11_stack_vs_heap/stack_vs_heap.md)
* [New 关键字](./NOTES/05_new/new.md)
* [智能指针 (Smart Pointers)](./NOTES/08_smart_ptr/smart_ptr.md)
* [移动语义 (Move Semantics)](./NOTES/25_Move_Semantics/Move_Semantics.md)

### 3. 数据类型与容器 (Data Types & Containers)
* [数组 (Old Array)](./NOTES/02_Arrays/Old_Array/README.md)
* [现代数组 (std::array)](./NOTES/02_Arrays/Modern_Array/Modern_Array.md)
* [多维数组](./NOTES/02_Arrays/Multidimensional_Array/Multidimensional_Array.md)
* [字符串 (String)](./NOTES/03_String/string/README.md)
* [字符串字面量深度解析](./NOTES/03_String/String_Literals/String_Literals_Deep_Dive.md)
* [基本数据类型](./NOTES/24_data_type/data_type.md)
* [联合体 (Union)](./NOTES/20_union/union.md)
* [迭代器 (Iterators)](./NOTES/26_iterators/iterators.md)

### 4. 现代 C++ 特性与泛型编程 (Modern C++ & Generic)
* [模板 (Templates)](./NOTES/10_Templates/Templates.md)
* [Auto 关键字](./NOTES/13_auto/auto.md)
* [Lambda 表达式](./NOTES/15_lambda/lambda.md)
* [多返回值 (Structured Binding)](./NOTES/09_Mutiple_return_values/09_Mutiple_return_values.md)

### 5. 高级语言特性 (Advanced Features)
* [Const 与 Mutable](./NOTES/04_Const_and_Mutable/Const_and_Mutable.md)
* [操作符重载](./NOTES/06_operators/operators.md)
* [作用域 (Scope)](./NOTES/07_Scope/Scope.md)
* [宏 (Macros)](./NOTES/12_macro/macro.md)
* [函数指针](./NOTES/14_func_ptr/func_ptr.md)
* [命名空间 (Namespace)](./NOTES/16_namespace/namespace.md)
* [多线程 (Thread)](./NOTES/17_thread/thread.md)
* [计时 (Timing)](./NOTES/18_timing/timing.md)
* [类型双关 (Type Punning)](./NOTES/19_type_punning/type_punning.md)

### 6. 开发工具与调试 (Tools & Debugging)
* [条件断点与操作断点](./NOTES/21_condition_action_breakpoints/condition_action_breakpoints.md)
* [预编译头文件](./NOTES/22_precompiled_headers/precompiled_headers.md)
* [性能基准测试 (Benchmarking)](./NOTES/23_Benchmarking/Benchmarking.md)
