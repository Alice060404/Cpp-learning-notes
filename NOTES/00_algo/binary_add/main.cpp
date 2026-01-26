#include<iostream>
#include"header.h"
#include<vector>
#include<string>
int main()
{
    std::string s1, s2;
    std::cout << "输入两个二进制字符串：" << std::endl;
    std::cin >> s1;
    std::cin >> s2;
    std::vector<int> v1 = str_to_vec(s1);   //string转vector
    std::vector<int> v2 = str_to_vec(s2);
    //调用加法函数
    std::vector<int> res = binary_add(v1, v2);
    //输出结果
    int len = res.size();
    for (int i = len-1; i >= 0;i--)
    {
        std::cout << res[i];
    }
    std::cout << std::endl;
    return 0;
}