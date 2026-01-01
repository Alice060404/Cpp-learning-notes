#include<vector>
#include"header.h"
#include<iostream>

//按位加法函数定义
std::vector<int> binary_add(const std::vector<int> &v1, 
                            const std::vector<int> &v2)
{
    int len1 = v1.size();
    int len2 = v2.size();
    std::vector<int> res;
    int t = 0;
    for (int i = 0; i < len1 || i < len2;i++)
    {
        if(i<len1)
            t += v1[i];
        if(i<len2)
            t += v2[i];
        res.push_back(t % 2);
        t /= 2;
    }
    if(t)
    {
        res.push_back(t);
    }
    return res;
}

//string转vector函数定义
std::vector<int> str_to_vec(const std::string s)
{
    std::vector<int> vec;
    int strlen = s.size();
    for (int i = strlen - 1; i >= 0; i--)
    {
        char c = s[i];
        vec.push_back(c - '0');
    }
    return vec;
}