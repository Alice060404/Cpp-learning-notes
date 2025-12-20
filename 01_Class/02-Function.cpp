#include<iostream>
class student
{
private:
    int id;
    int score;
public:
/*  student()
    {
        id=000;
        score=0;
    }*/
    student(int id,int score)       //普通构造函数
    {
        this->id = id;
        this->score = score;
        std::cout << "Making an object " << id << std::endl;
    }
    student(const student &s)       //复制构造函数，注意const和引用&
    {
        id = s.id;
        score = s.score;
        std::cout << "Copy " << s.id << std::endl;
    }
    ~student()      //注意：局部对象存放在栈上，先创建的对象先被销毁
    {
        std::cout << "Destroying" << id << std::endl;
    }
};
int main()
{
    // student s1; 编译错误，由于类中已经设置好构造函数，不会再有默认构造函数
    // 如果public中的函数没有被注释，student s1;合法    （函数重载）
    student s1(1001, 95);
    student s2(s1);     //调用系统默认的复制构造函数 （浅拷贝）克隆s1
    student s3(s1);     //调用复制构造函数  克隆s1
    student s4(1002, 90);   
    student s5(1003, 99);
    return 0;
}