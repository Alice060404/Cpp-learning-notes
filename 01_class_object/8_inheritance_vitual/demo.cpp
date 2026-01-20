/**
 * @file polymorphism_demo.cpp
 * @brief 演示 C++ 多态、纯虚函数、override 以及虚析构函数的完整实践
 * @note 场景：支付处理系统 (Payment System)
 */

#include <iostream>
#include <string>
#include <vector>

// ==========================================
// 1. 抽象基类 (Abstract Base Class)
// ==========================================
class PaymentMethod
{
protected:
    std::string ownerName;

public:
    PaymentMethod(std::string name) : ownerName(name) {}

    // 【重要拓展】虚析构函数
    // 确保通过基类指针删除对象时，子类的析构函数也能被调用，防止内存泄漏
    virtual ~PaymentMethod()
    {
        std::cout << "[Base Destructor] Cleaning up PaymentMethod for " << ownerName << std::endl;
    }

    // 纯虚函数 (Pure Virtual Function)
    // 定义接口：所有支付方式必须实现 "pay" 这个行为
    virtual void pay(double amount) const = 0;

    // 普通虚函数 (Virtual Function)
    // 子类可以选择重写，也可以直接用父类的
    virtual void printReceipt() const
    {
        std::cout << "Printing generic receipt for " << ownerName << std::endl;
    }
};

// ==========================================
// 2. 派生类 A: 信用卡支付
// ==========================================
class CreditCard : public PaymentMethod
{
private:
    std::string cardNumber;

public:
    CreditCard(std::string name, std::string number)
        : PaymentMethod(name), cardNumber(number) {}

    ~CreditCard() override
    {
        std::cout << "[Sub Destructor] Closing secure connection for CreditCard." << std::endl;
    }

    // 重写基类的纯虚函数 (实现多态的关键)
    void pay(double amount) const override
    {
        std::cout << "Processing Credit Card payment of $" << amount
                  << " using card ending in " << cardNumber.substr(cardNumber.length() - 4) << std::endl;
    }
};

// ==========================================
// 3. 派生类 B: 比特币支付
// ==========================================
class Bitcoin : public PaymentMethod
{
private:
    std::string walletAddress;

public:
    Bitcoin(std::string name, std::string addr)
        : PaymentMethod(name), walletAddress(addr) {}

    ~Bitcoin() override
    {
        std::cout << "[Sub Destructor] Clearing crypto keys from memory." << std::endl;
    }

    void pay(double amount) const override
    {
        std::cout << "Transferring " << (amount / 50000.0) << " BTC to network." << std::endl;
    }

    // 重写普通虚函数
    void printReceipt() const override
    {
        std::cout << "Generating Blockchain Proof for transaction." << std::endl;
    }
};

// ==========================================
// 4. 主函数：展示多态的威力
// ==========================================
int main()
{
    std::cout << "=== Polymorphism Demo: Payment System ===" << std::endl;

    // 容器存储基类指针 (Vector of Base Pointers)
    // 这是多态最常见的用法：用统一的容器管理不同的子类对象
    std::vector<PaymentMethod *> wallets;

    // 向上转型 (Upcasting)：子类对象指针 -> 基类指针
    wallets.push_back(new CreditCard("Alice", "1234-5678-9012-3456"));
    wallets.push_back(new Bitcoin("Bob", "1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa"));

    std::cout << "\n--- Processing Payments ---" << std::endl;

    // 遍历容器
    for (const auto *method : wallets)
    {
        // [动态绑定]
        // 编译器不知道 method 指向的是 CreditCard 还是 Bitcoin
        // 但在运行时，它会通过 V-Table 找到正确的 pay() 函数
        method->pay(100.0);
        method->printReceipt();
        std::cout << "-------------------------" << std::endl;
    }

    std::cout << "\n--- Cleaning Up Resources ---" << std::endl;

    // 清理内存
    for (auto *method : wallets)
    {
        // [虚析构函数的作用]
        // 如果基类析构函数不是 virtual，这里只会打印 [Base Destructor]
        // 而 [Sub Destructor] 不会执行，导致潜在内存泄漏
        delete method;
    }
    wallets.clear();

    return 0;
}