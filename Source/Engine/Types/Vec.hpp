#pragma once
#include <cmath>
#include "var_type.hpp"
#include <format>
#include "Utilities/FuncLib/GlobalMacros.hpp"
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
	constexpr auto& operator+=(const Vec2<U>& val) noexcept
    {
    	x += val.x;
    	y += val.y;
    	return *this;
    }
	template<typename U>
	constexpr auto& operator+=(U val) noexcept
    {
    	x += val;
    	y += val;
    	return *this;
    }
	template<typename U>

	constexpr auto& operator-=(const Vec2<U>& val) noexcept
    {
    	x -= val.x;
    	y -= val.y;
    	return *this;
    }
	template<typename U>
	constexpr auto& operator-=(U val) noexcept
    {
    	x -= val;
    	y -= val;
    	return *this;
    }

	template<typename U>
	constexpr auto operator+(const Vec2<U>& val) const noexcept
    {
    	using R = std::common_type_t<T, U>;
    	return Vec2<R>{x + val.x, y + val.y};
    }
	template<typename U>
	constexpr auto operator-(const Vec2<U>& val) const noexcept
    {
    	using R = std::common_type_t<T, U>;
    	return Vec2<R>{x - val.x, y - val.y};
    }

	// --- 数乘 ---
	template<typename U>
	constexpr auto operator*(U val) const noexcept {
    	using R = std::common_type_t<T, U>;
    	return Vec2<R>{x * val, y * val};
    }

	template<typename U>
	constexpr Vec2& operator*=(U val) noexcept {
    	x *= val;
    	y *= val;
    	return *this;
    }

	template<typename U>
	constexpr Vec2& operator*=(const Vec2<U>& val) noexcept {
    	x *= val.x;
    	y *= val.y;
    	return *this;
    }

	// --- 数除 ---
	template<typename U>
	constexpr auto operator/(U val) const noexcept
	{
    	using R = std::common_type_t<T, U>;
    	return Vec2<R>{x / val, y / val};
    }

	template<typename U>
	constexpr auto operator/(const Vec2<U>& val) const noexcept
	{
    	using R = std::common_type_t<T, U>;
    	return Vec2<R>{x / val.x, y / val.y};
    }

	template<typename U>
	constexpr Vec2& operator/=(U val) noexcept
	{
    	x /= val;
    	y /= val;
    	return *this;
    }

	template<typename U>
	constexpr Vec2& operator/=(const Vec2<U>& val) noexcept
	{
    	x /= val.x;
    	y /= val.y;
    	return *this;
    }
	bool operator==(Vec2<T> other)
    {
    	return x == other.x && y == other.y;
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
	// TODO 使用angle
	void RotateByAngle(float angle, Vec2<float> point)
    {
    	auto dx = x - point.x;
    	auto dy = y - point.y;
    	float x_new = dx * cos(PI) - dy * sin(PI);
    	float y_new = dx * sin(PI) + dy * cos(PI);
    	x += x_new;
    	y += y_new;
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
	template<typename U>
	Vec2<U> Cast()
    {
	    return Vec2<U>(x, y);
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