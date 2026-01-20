#include"High-precision_Adder.h"
#include<vector>
std::vector<int> add(std::vector<int> s1,std::vector<int> s2)
{
    int len1 = s1.size();
    int len2 = s2.size();
    std::vector<int> res;
    int t = 0;
    for (int i = 0; i < len1 || i < len2;i++)
    {
        if(i<len1)
            t += s1[i];
        if(i<len2)
            t += s2[i];
        res.push_back(t % 10);
        t /= 10;
    }
    if(t)
        res.push_back(t);
    return res;
}
