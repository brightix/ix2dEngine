#pragma once
#include <cmath>
#include "var_type.hpp"
#include <format>
template<typename T>
struct Vec2 : var_type
{
    T x;
    T y;
    Vec2() :x(0), y(0) {}

    Vec2(T x,T y)
    {
        this->x = x;
        this->y = y;
    }
    
    template<typename U>
    explicit Vec2(Vec2<U>& V)
    {
        x = static_cast<T>(V.x);
        y = static_cast<T>(V.y);
    }
	template<typename U>
	Vec2(Vec2&& V) noexcept
	{
    	x = static_cast<T>(V.x);
    	y = static_cast<T>(V.y);
	}

	template<typename U>
	auto operator+=(U val) const
    {
    	using R = decltype(x - val); // 自动推导结果类型
    	return Vec2<R>(x - val, y - val);
    }
	template<typename U>
	auto operator+=(Vec2<U> val) const
    {
    	using R = decltype(x - val.x); // 自动推导结果类型
    	return Vec2<R>(x - val.x, y - val.y);
    }

	template<typename U>
	auto operator-=(U val)
    {
    	x-=val;
    	y-=val;
    }
	template<typename U>
	auto operator-=(Vec2<U> val)
    {
    	x-=val.x;
    	y-=val.y;
    }

	template<typename U>
	auto operator*(U val) const
    {
    	using R = decltype(x * val); // 自动推导结果类型
    	return Vec2<R>(x * val, y * val);
    }
	// template<typename U>
	// Vec2<U> operator*(U val)
 //    {
	//     return Vec2(x*val,y*val);
 //    }
	Vec2<float> operator/(float val)
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
	Vec2<float> Normalize()
    {
    	float len = Length();
    	if (len == 0)
    	{
    		return {0,0};
    	}
    	return *this/len;
    }
	void Reset()
    {
	    x = T{};
    	y = T{};
    }
	std::string str() override
    {
    	return std::format("Vec.x: {}，Vec.y: {}", x, y);
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