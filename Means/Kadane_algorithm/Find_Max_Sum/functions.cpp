#include"header.h"
#include<vector>
#include<algorithm>
#include<iostream>
std::vector<int> getdata()
{
    std::cout << "请输入原数组长度：";
    int len;
    std::cin >> len;
    std::cout << "输入原数组元素：";
    std::vector<int> nums;
    for (int i = 0; i < len; i++)
    {
        int data;
        std::cin >> data;
        nums.push_back(data);
    }
    return nums;
}
int func(std::vector<int>nums)
{
    if(nums.empty())
        return 0;
    int max_sum = nums[0];
    int current_sum = 0;
    for(int x:nums)
    {
        if(current_sum<0)
            current_sum = x;
        else
            current_sum += x;
        max_sum = std::max(max_sum, current_sum);
    }
    return max_sum;
}