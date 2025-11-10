#pragma once
#include "var_type.hpp"
#include <format>
#include "Utilities/FuncLib/GlobalMacros.hpp"
template<typename T>
struct Vec2
{
    T x;
    T y;
	constexpr Vec2() noexcept : x(T{}), y(T{}) {}

	// constexpr 构造
	constexpr Vec2(T _x, T _y) noexcept : x(_x), y(_y) {}

	//constexpr Vec2(T&& _x, T&& _y) noexcept : x(std::forward<T>(_x)), y(std::forward<T>(_y)) {}
    
    template<typename U>
    Vec2(Vec2<U>& V)
    {
        x = static_cast<T>(V.x);
        y = static_cast<T>(V.y);
    }
	Vec2(Vec2<T>&& V) noexcept
	{
    	x = static_cast<T>(V.x);
    	y = static_cast<T>(V.y);
	}
	template<typename U>
	Vec2(Vec2<U>&& V) noexcept
    {
    	x = static_cast<T>(V.x);
    	y = static_cast<T>(V.y);
    }

	template<typename U>
	Vec2(const Vec2<U>& V)
    {
    	x = static_cast<T>(V.x);
    	y = static_cast<T>(V.y);
    }
	Vec2(const Vec2& other) = default;


	Vec2& operator=(const Vec2& other)
	{
		x = other.x;
		y = other.y;
		return *this;
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
	constexpr auto operator*(Vec2<U> val) const noexcept {
		using R = std::common_type_t<T, U>;
		return Vec2<R>{x * val.x, y * val.y};
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

	// 求长度平方
	[[nodiscard]] constexpr T LengthSquared() const noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		return x * x + y * y;
	}

	// 求长度
	[[nodiscard]] T Length() const noexcept
	{
		return std::sqrt(LengthSquared());
	}

	// 归一化（返回 Vec2<T>）
	[[nodiscard]] Vec2 Normalized(T precision = static_cast<T>(0.00001)) const noexcept
	{
		static_assert(std::is_arithmetic_v<T>);
		T len = Length();
		if (len < precision)
			return Vec2{}; // 返回 0 向量

		return *this / len;
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
	std::string Str()
    {
    	return std::format("Vec.x: {}，Vec.y: {}", x, y);
    }
	template<typename U>
	Vec2<U> Cast()
    {
	    return Vec2<U>(x, y);
    }

    Vec2<float> operator-()
	{
		return Vec2<float>{-x, -y};
	}
};


template<typename T>
struct std::formatter<Vec2<T>> {
	constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const Vec2<T>& p, FormatContext& ctx) {
		return format_to(ctx.out(), "(x: {}, y: {})", p.x, p.y);
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
	inline float Dot2(Vec2<float> a,Vec2<float> b)
	{
		return a.x * b.x+a.y * b.y;
	}
}