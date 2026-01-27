[MindMap](./Visibility_and_Encapsulation.png)

[demo](./demo.cpp)


# C++ 学习笔记：可见性与访问控制 (Visibility & Access Control)

> **核心哲学**：可见性是写给**程序员**看的，而不是写给 CPU 看的。
> 它是一种**编译时 (Compile-time)** 的约束机制，用于在代码层面划分“内部实现”与“外部接口”，对程序的运行性能没有任何影响（Zero Overhead）。

## 1. 为什么需要控制可见性？ (The "Why")
在思维导图中提到了“代码健壮性”，这背后的深层原因是维护**类的不变量 (Class Invariants)**。

* **保护内部状态**：如果一个成员变量（如 `health`）被设为 `public`，外部代码可以将其设为 `-100`，这违反了游戏逻辑。通过设为 `private` 并提供 `takeDamage()` 方法，我们可以确保血量永远逻辑正确。
* **降低耦合 (Decoupling)**：隐藏实现细节（Private），只暴露接口（Public）。这意味着你以后修改内部算法或数据结构时，不需要修改调用它的外部代码。
* **API 设计**：`public` 部分构成了你的 API。越少的 `public` 成员意味着用户越容易上手（不用猜测哪个函数才是该调用的）。

---

## 2. 三大访问修饰符详解 (Access Modifiers)

###  Private (私有) - "我的隐私"
* **权限**：只有**类本身**的成员函数（以及友元）可以访问。
* **适用场景**：
    * 类的内部状态变量（如 `size`, `capacity`）。
    * 辅助函数（Helper Functions），仅供类内部调用，不希望对外暴露。
* **默认规则**：`class` 的成员默认是 `private`。

###  Protected (受保护) - "家族传承"
* **权限**：
    * **类本身**可以访问。
    * **子类 (Derived Classes)** 可以访问。
    *  **外部代码**（包括 `main` 函数）不可访问。
* **适用场景**：希望被子类复用或修改，但不对外公开的逻辑。例如基类的 `initInternal()` 函数。
* **注意**：`protected` 是继承机制的基石。

###  Public (公共) - "对外接口"
* **权限**：任何代码（类内部、子类、外部函数）均可访问。
* **适用场景**：类的**接口 (Interface)**。即你希望别人如何使用这个类。
* **默认规则**：`struct` 的成员默认是 `public`。

---

## 3. 特殊例外：友元 (Friend)
思维导图中提到了 `friend`，这是 C++ 特有的“后门”机制。

* **定义**：允许指定的**外部函数**或**其他类**访问当前类的 `private` 和 `protected` 成员。
* **使用场景**：
    * **操作符重载**：如 `friend std::ostream& operator<<`，为了让 `cout` 能直接打印对象私有数据。
    * **紧密协作的类**：如 `LinkedList` 和 `Node`，或者 `UnitTest`（单元测试）需要检查私有状态时。
* **警示**：友元破坏了封装性，**慎用**。它会产生强耦合。

---

## 4. 开发中的最佳实践 (Best Practices)
1.  **默认 Private**：先把所有成员设为 `private`，只有当你确定需要公开时，才将其移到 `public`。
2.  **避免 Public 数据成员**：除了简单的 `struct` (POD 类型)，尽量不要把变量设为 `public`。使用 `Getter` (Get方法) 提供只读访问。
3.  **谨慎使用 Getter/Setter**：如果你的类全是 `getX()` 和 `setX()`，那它只是一个披着 Class 外衣的 Struct。真正的封装是提供**行为**（如 `bank.deposit()`）而不是数据（如 `bank.money += 100`）。