#pragma once
#include <SDL3/SDL_rect.h>
#include "Vec.hpp"

class FRect
{
public:
    SDL_FRect rect;

    FRect() { rect = {0.f, 0.f, 0.f, 0.f}; }
    FRect(float x, float y, float w, float h)
    {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
    }
    ~FRect() = default;

    // 判断是否完全包含另一个矩形
    bool contains(const FRect& other) const
    {
        return (other.rect.x >= rect.x) &&
               (other.rect.x + other.rect.w <= rect.x + rect.w) &&
               (other.rect.y >= rect.y) &&
               (other.rect.y + other.rect.h <= rect.y + rect.h);
    }

    // 判断是否与另一个矩形相交
    bool intersects(const FRect& other) const
    {
        return !(other.rect.x + other.rect.w < rect.x ||
                 other.rect.x > rect.x + rect.w ||
                 other.rect.y + other.rect.h < rect.y ||
                 other.rect.y > rect.y + rect.h);
    }
};