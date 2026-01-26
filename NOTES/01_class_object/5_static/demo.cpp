/**
 * @file static_demo.cpp
 * @brief 演示 static 在类内部（共享数据）和类外部（内部链接）的用法
 */

#include <iostream>

// ==========================================
// 1. 类外部的 Static (Internal Linkage)
// ==========================================
// 这个变量只能在这个文件中被访问，其他文件即使 extern 也找不到它
static int fileLocalVar = 999;

// ==========================================
// 2. 类内部的 Static (Shared Member)
// ==========================================
class User
{
private:
    std::string username;

    // 静态变量声明：所有 User 对象共用这个计数器
    static int userCount;

public:
    User(std::string name) : username(name)
    {
        userCount++; // 每创建一个对象，总数+1
    }

    ~User()
    {
        userCount--; // 每销毁一个对象，总数-1
    }

    // 静态方法：不需要对象就能调用
    static int getUserCount()
    {
        // return username; // ? 错误！静态方法不能访问非静态成员
        return userCount; // ? 正确
    }
};

// 【重要】静态成员变量必须在类外进行定义和初始化
int User::userCount = 0;

int main()
{
    // -----------------------------------
    // 测试静态方法 (无需实例化)
    // -----------------------------------
    std::cout << "Initial User Count: " << User::getUserCount() << std::endl;

    // -----------------------------------
    // 测试静态变量 (共享性)
    // -----------------------------------
    User *u1 = new User("Alice");
    User u2("Bob");

    std::cout << "Count after creating Alice & Bob: " << User::getUserCount() << std::endl;

    delete u1; // 销毁 Alice

    std::cout << "Count after deleting Alice: " << u2.getUserCount() << std::endl;
    // 注：也可以用对象 u2.getUserCount() 访问，但推荐用类名 User::

    return 0;
}