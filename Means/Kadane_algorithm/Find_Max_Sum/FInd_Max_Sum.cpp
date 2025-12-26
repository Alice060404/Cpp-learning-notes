#include<iostream>
#include<vector>
#include<algorithm>
#include"header.h"
int main()
{
    std::vector<int> nums = getdata();
    std::cout << "最大子数组和为：" << func(nums) << std::endl;
    return 0;
}