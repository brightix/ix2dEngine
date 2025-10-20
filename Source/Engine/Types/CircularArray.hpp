#pragma once
#include <vector>


template<typename T>
class CircularArray
{
	std::vector<T> data;
	int head,tail;
public:
    CircularArray(size_t cap = 10) : head(-1), tail(0)
	{
		data.resize(cap);
	}
	bool insert(T val)
    {
	    if (head < tail)
	    {
		    data[tail] = val;
	    	tail = (tail + 1) % data.size();
	    }
    	return true;
    }



    ~CircularArray()= default;
};

