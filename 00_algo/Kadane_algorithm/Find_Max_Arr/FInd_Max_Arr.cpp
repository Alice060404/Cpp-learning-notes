#include<iostream>
#include<vector>
#include<algorithm>
int main()
{
    std::cout << "请输入原数组长度：";
    int len;
    std::cin >> len;
    std::cout << "输入数组元素：";
    std::vector<int> nums;
    for (int i = 0; i < len; i++)
    {
        int data;
        std::cin >> data;
        nums.push_back(data);
    }
    return 0;
}