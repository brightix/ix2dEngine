#pragma once
#include <cmath>
template<typename T>
struct Vec2d
{
    T x;
    T y;
    Vec2d()= default;

    Vec2d(T x,T y)
    {
        this->x = x;
        this->y = y;
    }
    
    template<typename U>
    explicit Vec2d(Vec2d<U>& V)
    {
        x = static_cast<T>(V.x);
        y = static_cast<T>(V.y);
    }
	//template<type>
	void operator+=(Vec2d<T> other)
    {
	    x+=other.x;
    	y+=other.y;
    }
	template<typename U>
	Vec2d<U> operator*(U val)
    {
	    return Vec2d<U>(x*val,y*val);
    }
	Vec2d<float> operator/(float val)
    {
    	if constexpr (std::is_fundamental_v<T>)
    	{
    		return {x/val,y/val};
    	}
    	return {0.f,0.f};
    }
	//Attr
	float Length()
    {
    	return sqrt(x*x + y*y);
    }

	//algorithm
	Vec2d<float> Normalize()
    {
    	float len = Length();
    	if (len == 0)
    	{
    		return {0,0};
    	}
    	return *this/len;
    }
};

template<typename T>
struct Vec
{
    T x;
    T y;
    T z;
    Vec()=  default;
    Vec(T x,T y,T z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

};

namespace Math
{

}