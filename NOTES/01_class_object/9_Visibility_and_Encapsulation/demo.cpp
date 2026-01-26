/**
 * @file visibility_demo.cpp
 * @brief 演示 C++ 访问修饰符 (public, protected, private) 及 friend 机制
 * @note 场景：一个模拟的数据库连接类
 */

#include <iostream>
#include <string>

// 前向声明，告诉编译器 "SystemAdmin" 这个类稍后会定义
class SystemAdmin;

class DBConnection
{
private:
    // 【Private】: 绝对隐私，只有自己能看
    // 外部无法修改端口或密钥，保证连接安全
    std::string secretKey;
    int port;

protected:
    // 【Protected】: 家族传承，子类可用
    // 比如一个特定的 "MySQLConnection" 子类可能需要访问这个状态
    std::string connectionState;

    void resetConnection()
    {
        connectionState = "DISCONNECTED";
        std::cout << "[Protected] Connection reset internal logic." << std::endl;
    }

public:
    // 【Public】: 对外接口
    std::string dbName; // 也可以暴露一些非关键数据

    DBConnection(std::string name, std::string key)
        : dbName(name), secretKey(key), port(3306), connectionState("READY") {}

    // 公共方法提供服务
    void connect()
    {
        // 类内部可以访问 private 和 protected
        std::cout << "Connecting to " << dbName << " on port " << port
                  << " using key: [HIDDEN]" << std::endl;
        connectionState = "CONNECTED";
    }

    // 声明友元类：允许 SystemAdmin 访问我的所有隐私
    friend class SystemAdmin;
};

// 子类：演示 Protected 权限
class MySQLConnection : public DBConnection
{
public:
    MySQLConnection(std::string name, std::string key) : DBConnection(name, key) {}

    void debugStatus()
    {
        // ? OK: 子类可以访问父类的 protected 成员
        std::cout << "Debug Info: State is " << connectionState << std::endl;

        // ? Error: 子类无法访问父类的 private 成员
        // std::cout << "Port is " << port << std::endl;
    }
};

// 友元类：拥有最高权限
class SystemAdmin
{
public:
    void forceChangePort(DBConnection &db, int newPort)
    {
        // ? OK: 作为 friend，可以访问 private 成员
        std::cout << "[Admin] Forcing port change from " << db.port << " to " << newPort << std::endl;
        db.port = newPort;
    }

    void viewSecret(DBConnection &db)
    {
        // ? OK: friend 可以查看 private 密钥
        std::cout << "[Admin] Viewing secret key: " << db.secretKey << std::endl;
    }
};

int main()
{
    DBConnection db("UserDB", "xk8-29a-vz1");
    SystemAdmin admin;

    // 1. Public 访问
    std::cout << "--- Public Access ---" << std::endl;
    db.connect();                                       // ? OK
    std::cout << "DB Name: " << db.dbName << std::endl; // ? OK

    // 2. Private 访问尝试
    // std::cout << "Port: " << db.port << std::endl; // ? 编译错误：private 不可访问
    // std::cout << "Key: " << db.secretKey << std::endl; // ? 编译错误：private 不可访问

    // 3. Protected 访问尝试
    // db.resetConnection(); // ? 编译错误：protected 对外部不可见（仅限子类）

    // 4. Friend 权限展示
    std::cout << "\n--- Friend Access ---" << std::endl;
    admin.viewSecret(db);            // 通过友元类访问 private 数据
    admin.forceChangePort(db, 8080); // 通过友元类修改 private 数据

    // 5. 子类访问展示
    std::cout << "\n--- Subclass Access ---" << std::endl;
    MySQLConnection mySqlDb("OrderDB", "mysql-key-999");
    mySqlDb.debugStatus(); // 子类函数内部访问了 protected 成员

    return 0;
}