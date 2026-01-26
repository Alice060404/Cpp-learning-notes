#include <iostream>

// 对应导图：类 (Class) 的定义 - 自定义数据类型
class Player
{
    // 对应导图：可见性/访问权限 - 默认权限：私有 (Private)
    // 通常将数据设为私有，以控制代码安全性
private:
    // 对应导图：类成员 - 数据 (变量)
    // 示例：位置 X, Y 和 移动速度
    int x;
    int y;
    int speed;

    // 对应导图：可见性/访问权限 - 公有 (Public)
public:
    // 构造函数：用于初始化对象 (虽然导图没细讲，但这是初始化的标准写法)
    Player(int startX, int startY, int startSpeed)
    {
        x = startX;
        y = startY;
        speed = startSpeed;
    }

    // 对应导图：类成员 - 功能 (方法)
    // 特点：类内部定义的函数，操作类内部数据
    void move(int dx, int dy)
    {
        // 直接操作内部数据 x, y，无需像C语言那样传指针
        x += dx * speed;
        y += dy * speed;
        std::cout << "[操作] 玩家移动了..." << std::endl;
    }

    // 功能：展示当前状态
    void displayStatus()
    {
        std::cout << "当前状态 -> 位置: (" << x << ", " << y
                  << "), 速度: " << speed << std::endl;
    }
}; // 对应导图：结尾需加分号

int main()
{
    // 对应导图：对象 (Object) - 类的实例
    // 变量实例化
    Player myPlayer(0, 0, 2); // 创建一个在(0,0)，速度为2的玩家对象

    std::cout << "--- 初始状态 ---" << std::endl;
    // 对应导图：对象 - 点运算符 (.) 访问成员
    myPlayer.displayStatus();

    std::cout << "\n--- 移动后 ---" << std::endl;
    // 调用公有方法来操作私有数据
    myPlayer.move(1, 1); // 这里的参数被简化了，不需要传对象本身的指针
    myPlayer.displayStatus();

    return 0;
}