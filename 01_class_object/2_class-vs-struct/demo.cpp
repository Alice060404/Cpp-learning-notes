/**
 * @file class_vs_struct.cpp
 * @brief 演示 C++ 中 Class 和 Struct 的区别及惯用写法
 * @note 适合 C++ 初学者参考
 */

#include <iostream>
#include <string>
#include <cmath>

// ==========================================
// 1. Struct 示例：简单的数据结构 (POD)
// ==========================================
// 惯例：用于数学向量、配置项等简单数据
struct Vec2
{
    float x; // 默认为 public，外部可以直接访问
    float y;

    // Struct 中也可以有方法，通常比较简单
    float length() const
    {
        return std::sqrt(x * x + y * y);
    }
};

// ==========================================
// 2. Class 示例：复杂的功能实体
// ==========================================
// 惯例：用于玩家、管理器等需要封装逻辑的对象
class Player
{
    // 默认为 private，外部无法直接修改，保护了数据的安全性
    std::string name;
    int health;
    int maxHealth;

public:
    // 构造函数：初始化复杂状态
    Player(std::string n, int hp) : name(n), health(hp), maxHealth(hp) {}

    // 公有方法：提供操作数据的接口
    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
            health = 0;
        std::cout << "[Class Log] " << name << " took damage. HP: " << health << std::endl;
    }

    // Getter 方法
    int getHealth() const { return health; }
};

// ==========================================
// 3. 核心区别演示：默认访问权限
// ==========================================
struct TestStruct
{
    int id; // 默认 public
};

class TestClass
{
    int id; // 默认 private
};

int main()
{
    // -----------------------------------
    // 场景 A: 使用 Struct (类似于 C 语言风格的数据包)
    // -----------------------------------
    Vec2 position = {3.0f, 4.0f}; // 可以直接聚合初始化
    std::cout << "--- Struct Usage ---" << std::endl;
    // 直接访问成员变量（方便，但缺乏封装）
    std::cout << "Position: (" << position.x << ", " << position.y << ")" << std::endl;
    std::cout << "Length: " << position.length() << std::endl;

    std::cout << std::endl;

    // -----------------------------------
    // 场景 B: 使用 Class (面向对象封装)
    // -----------------------------------
    Player p1("Hero", 100);
    std::cout << "--- Class Usage ---" << std::endl;
    // p1.health = 50; // ? 编译错误！无法访问 private 成员
    p1.takeDamage(20); // ? 必须通过 public 接口操作
    std::cout << "Current HP: " << p1.getHealth() << std::endl;

    // -----------------------------------
    // 场景 C: 默认权限测试
    // -----------------------------------
    TestStruct ts;
    ts.id = 1; // ? OK: Struct 默认为 public

    TestClass tc;
    // tc.id = 1; // ? Error: Class 默认为 private (如果不手动写 public:)

    return 0;
}