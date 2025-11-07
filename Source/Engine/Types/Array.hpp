#pragma once
#include <vector>

template<typename T>
class Array
{
public:
    std::vector<T> data;
    Array()= default;

    void remove(const T& to_remove)
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

    void insert(T& element)
    {
        data.emplace_back(element);
    }

    template<typename U>
    void insert(U* value) requires std::is_base_of_v<std::remove_pointer_t<T>, U>
    {
        data.push_back(static_cast<T>(value));
    }



    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto cbegin() const { return data.cbegin(); }
    auto cend() const { return data.cend(); }
    ~Array()= default;
};
