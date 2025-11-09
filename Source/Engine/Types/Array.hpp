#pragma once
#include <vector>

template<typename T>
class Array
{
public:
    std::vector<T> data;
    Array()= default;

    bool remove(const T& to_remove)
    {
        const int n = data.size();
        for (size_t i = 0; i < n; ++i)
        {
            if (data[i] == to_remove)
            {
                // 用最后一个元素覆盖当前
                data[i] = std::move(data.back());
                data.pop_back(); // 删除最后一个
                return true;
            }
        }
    	return false;
    }

	std::vector<T>::iterator Find(const T& target)
    {
		return std::find(data.begin(), data.end(), target);
    }


    void insert(const T& element)
    {
        data.emplace_back(element);
    }

    template<typename U>
    void insert(U value) requires std::is_base_of_v<std::remove_pointer_t<T>, U>
    {
        data.push_back(static_cast<T>(value));
    }



	T& operator[](const int index) {
		return data[index];
    }
//迭代器
    auto begin() { return data.begin(); }
    auto end() { return data.end(); }
    auto begin() const { return data.begin(); }
    auto end() const { return data.end(); }
    auto cbegin() const { return data.cbegin(); }
    auto cend() const { return data.cend(); }
    ~Array()= default;
};
