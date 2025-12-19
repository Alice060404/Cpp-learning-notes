#include<iostream>
using namespace std;
//定义类
class student
{
private:        //私有，不能随意直接更改
    int id;
    int score;
public:         //公共，能直接访问
    int setScore(int s)        //函数：限定分数范围
    {
        if(s>=0&&s<=100)
            return s;
        else
            cout << "error" << endl;
    }
    void setInfo(int id,int score)      //函数：获取数据
    {
        this->id = id;          //用this指针区别形参
        this->score = setScore(score);      //调用成员函数
    }
    void show()         //函数：输出数据
    {
        cout << id << " " << score << endl;
    }
};              //注意有个分号，与结构体类似

int main()
{
    student s1, s2; // 定义对象
    int id, score;
    cin >> id >> score;
    s1.setInfo(id, score);      //输入数据
    cin >> id >> score;
    s2.setInfo(id, score);
    s1.show();              //展示数据
    //s1.id=100         //编译不通过，因为id是私有成员
}