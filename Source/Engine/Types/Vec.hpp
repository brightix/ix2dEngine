#pragma once
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