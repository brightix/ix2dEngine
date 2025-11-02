#pragma once
#include <SDL3/SDL_rect.h>
#include "Vec.hpp"
#include "Enums/CollisionType.hpp"

struct FRect
{
	float x;
	float y;
	float w;
	float h;

	FRect()
		: x(0.f), y(0.f), w(0.f), h(0.f) {}
	// FRect(float inX, float inY, float inW, float inH)
	// 	: x(inX), y(inY), w(inW), h(inH) {}
	template<typename T1, typename T2, typename T3, typename T4>
	FRect(T1 inX, T2 inY, T3 inW, T4 inH)
		: x(static_cast<float>(inX)),
		  y(static_cast<float>(inY)),
		  w(static_cast<float>(inW)),
		  h(static_cast<float>(inH)) {}
	~FRect() = default;

	// 判断是否完全包含另一个矩形
	bool contains(const FRect& other) const
	{
		return (other.x >= x) &&
			   (other.x + other.w <= x + w) &&
			   (other.y >= y) &&
			   (other.y + other.h <= y + h);
	}
	RectCollisionTypeNoRot CollisionDir(const FRect& other) const
	{
		float dx = (x + w / 2) - (other.x + other.w / 2); // 中心X差
		float dy = (y + h / 2) - (other.y + other.h / 2); // 中心Y差

		float combinedHalfWidth = (w + other.w) / 2;
		float combinedHalfHeight = (h + other.h) / 2;

		float overlapX = combinedHalfWidth - abs(dx);
		float overlapY = combinedHalfHeight - abs(dy);

		// if (overlapX < 0 || overlapY < 0)
		// 	return NONE; // 没有碰撞

		if (overlapX < overlapY) {
			// 横向碰撞
			return dx > 0 ? LEFT : RIGHT; // A在other右侧碰到other左边→碰撞方向是LEFT
		} else {
			// 纵向碰撞
			return dy > 0 ? TOP : BOTTOM; // A在other下方碰到other上边→碰撞方向是TOP
		}
	}

	// 判断是否与另一个矩形相交
	bool intersects(const FRect& other) const
	{
		return !(other.x + other.w < x ||
				 other.x > x + w ||
				 other.y + other.h < y ||
				 other.y > y + h);
	}

	// 获取右边缘
	float right() const { return x + w; }

	// 获取下边缘
	float bottom() const { return y + h; }

	SDL_FRect CastToSDL_FRect() const
	{
		return SDL_FRect(x,y,w,h);
	}
};
