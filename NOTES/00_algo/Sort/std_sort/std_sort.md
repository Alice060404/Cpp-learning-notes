[MindMap](./std_sort.png)


# C++ 排序算法详解 (`std::sort`)

## 1. 核心观念：不要重复造轮子

* **手动排序**：冒泡、选择、插入排序。时间复杂度通常是 。作为算法练习可以写，但在实际工程中**严禁使用**。
* **`std::sort`**：位于 `<algorithm>` 头文件中。
* **时间复杂度**：平均 。
* **底层实现**：通常使用 **IntroSort (内省排序)**。它首先进行快速排序（QuickSort），如果递归太深则切换为堆排序（HeapSort），对于小数组则使用插入排序（Insertion Sort）。
* **适用范围**：支持随机访问迭代器的容器（如 `std::vector`, `std::array`, 原生数组），**不适用**于 `std::list`（链表有自己的 `.sort()` 成员函数）。



## 2. 基础语法

`std::sort(start_iterator, end_iterator, [predicate]);`

* **前两个参数**：定义了要排序的范围 `[start, end)`（左闭右开）。
* `vector`: `v.begin(), v.end()`
* 数组: `arr, arr + size`


* **第三个参数 (可选)**：**谓词 (Predicate)**。
* 如果不填：默认使用 `<` 运算符进行**升序**排列。
* 如果填了：使用你提供的函数来判断“谁排在前面”。



## 3. 自定义排序规则 (The Power of Predicates)

这是 `std::sort` 最强大的地方。你不需要修改数据结构，只需要改变“比较规则”。

### 3.1 内置仿函数

* `std::greater<T>()`：实现**降序**排列。

### 3.2 Lambda 表达式 (推荐)

* 最灵活的方式。
* 规则：函数返回 `true` 表示 `a` 应该排在 `b` **前面**（即 `a < b` 的语义）。
* 升序：`return a < b;`
* 降序：`return a > b;`



### 3.3 运算符重载

* 如果在类内部重载了 `operator<`，那么直接调用 `std::sort` 即可，无需传第三个参数。

---

## 4. 代码实战笔记

```cpp
/*
 * 文件名: sorting_demo.cpp
 * 描述: 演示 std::sort 的基本用法、Lambda 自定义排序及结构体排序
 * 编译: g++ -o sort_test sorting_demo.cpp -std=c++14
 */

#include <iostream>
#include <vector>
#include <algorithm> // 必须包含，std::sort 在这里
#include <functional> // for std::greater
#include <string>

using namespace std;

// 定义一个自定义结构体
struct Student {
    string name;
    int score;
    int age;

    // 打印辅助函数
    void print() const {
        cout << "  {Name: " << name << ", Score: " << score << ", Age: " << age << "}" << endl;
    }

    // 方法3：重载 < 运算符
    // 让 Student 对象天生支持“按分数升序”
    bool operator<(const Student& other) const {
        return this->score < other.score;
    }
};

// 辅助函数：打印容器
template<typename T>
void PrintVector(const vector<T>& v, const string& comment) {
    cout << comment << ": ";
    for (const auto& item : v) cout << item << " ";
    cout << endl;
}

int main() {
    cout << "=== 1. Basic Sorting (Integers) ===" << endl;
    vector<int> numbers = {5, 2, 9, 1, 5, 6};
    
    // 默认升序 (Ascending)
    // 时间复杂度: O(N log N)
    std::sort(numbers.begin(), numbers.end());
    PrintVector(numbers, "Sorted (Asc)");

    // 降序 (Descending) 使用内置仿函数
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());
    PrintVector(numbers, "Sorted (Desc)");


    cout << "\n=== 2. Advanced Sorting (Custom Objects) ===" << endl;
    vector<Student> classA = {
        {"Alice", 85, 20},
        {"Bob", 95, 19},
        {"Charlie", 85, 21}, // 分数相同，年龄不同
        {"Dave", 60, 22}
    };

    // 场景 A: 默认排序 (使用了 operator< 重载)
    // 规则：按分数升序
    std::sort(classA.begin(), classA.end());
    cout << ">> Sorted by Score (Asc) using operator<:" << endl;
    for(const auto& s : classA) s.print();


    // 场景 B: 使用 Lambda 自定义复杂逻辑
    // 规则：按分数降序；如果分数相同，按年龄升序 (更年轻的排前面)
    std::sort(classA.begin(), classA.end(), [](const Student& a, const Student& b) {
        if (a.score == b.score) {
            return a.age < b.age; // 分数相同，比年龄
        }
        return a.score > b.score; // 分数不同，比分数 (降序)
    });

    cout << "\n>> Sorted by Score(Desc) then Age(Asc) using Lambda:" << endl;
    for(const auto& s : classA) s.print();


    cout << "\n=== 3. Optimization Tip ===" << endl;
    // 如果你只需要找出“前 3 名”，不需要对全班排序
    // 可以使用 std::partial_sort (部分排序)，它比 std::sort 更快
    // 语法: partial_sort(开始, 排序终点, 结束, 规则)
    
    std::partial_sort(classA.begin(), classA.begin() + 2, classA.end(), 
        [](const Student& a, const Student& b) {
            return a.score > b.score;
    });

    cout << ">> Top 2 Students (Partial Sort):" << endl;
    classA[0].print();
    classA[1].print();

    return 0;
}

```

### 学习要点 (Key Takeaways)

1. **左闭右开**：`numbers.end()` 指向的是最后一个元素**之后**的位置，这是 STL 迭代器的通用规则。
2. **Strict Weak Ordering**：你提供的比较函数必须满足“严格弱序”。简单来说，`cmp(a, a)` 必须永远返回 `false`（不能是 `>=`），否则在某些极端情况下会导致 sort 死循环或崩溃。
3. **部分排序优化**：如果面试官问你“如何在大海量数据中只取前 K 个最大值”，不要回答全排序，要回答使用 **堆 (Heap)** 或者 `std::partial_sort` / `std::nth_element`。