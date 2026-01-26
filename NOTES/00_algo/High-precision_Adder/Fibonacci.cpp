//以斐波那契数列为例

#include<vector>
#include<iostream>
#include<chrono>
#include"High-precision_Adder.h"
using namespace std::chrono;
int main()
{
    int n;
    while(std::cout<<"请输入项数n: ", std::cin>>n)
    {
        if(n==0)
            std::cout << "第0项为: 0" << std::endl;
        if(n==1)
            std::cout << "第1项为: 1" << std::endl;
        std::vector<int> pre_1 = {1};
        std::vector<int> pre_2 = {0};
        std::vector<int> current;
        auto start = high_resolution_clock::now();
        for (int i = 0; i < n - 1;i++)
        {
            current = add(pre_1, pre_2);
            pre_2 = pre_1;
            pre_1 = current;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        int len = current.size();
        std::cout << "第" << n << "项是: ";
        for (int i = len - 1; i >= 0;i--)
        {
            std::cout << current[i] << (i == 0 ? "\n" : "");
        }
        std::cout << "本次计算耗时：" << duration.count() << " us(微秒)" << std::endl;
    }
    return 0;
}