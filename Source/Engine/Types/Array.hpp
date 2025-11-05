#pragma once
#include <vector>

template<typename T>
class Array
{
public:
    std::vector<T> data;
    Array()= default;
    void remove(T& to_remove)
    {
        const int n = data.size();
        for (size_t i = 0; i < n; ++i)
        {
            if (data[i] == to_remove)
            {
                // 用最后一个元素覆盖当前
                data[i] = data.back();
                data.pop_back(); // 删除最后一个
                break;
            }
        }
    }
    ~Array()= default;
};
