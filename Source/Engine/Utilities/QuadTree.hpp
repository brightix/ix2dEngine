#pragma once

#if 1
// 如果定义了 DISABLE_THIS_FILE，就不编译下面内容
#include <functional>
#include <memory>

#include "Types/FRect.hpp"
#include <vector>

class QuadTree
{
    static constexpr int MAX_OBJECTS = 4; // 每个节点最大存储对象数
    static constexpr int MAX_LEVELS = 5;  // 最大深度

    int level;
    bool is_infinite;
    FRect boundary;
    std::vector<SPhysicsBaseUtility*> objects;
    std::unique_ptr<QuadTree> nw, ne, sw, se;
    std::function<void(SPhysicsBaseUtility*,SPhysicsBaseUtility*)> collision_callback;
public:
    QuadTree() : level(0), is_infinite(false)
    {
    }

    void SetCollisionStrategy(const std::function<void(SPhysicsBaseUtility*,SPhysicsBaseUtility*)>& callback)
    {
        collision_callback = callback;
    }
    ~QuadTree(){}
    // 分裂四个子节点
    void Subdivide()
    {
        float x = boundary.rect.x;
        float y = boundary.rect.y;
        float w = boundary.rect.w / 2.f;
        float h = boundary.rect.h / 2.f;

        nw = std::make_unique<QuadTree>(level + 1, FRect(x, y, w, h));
        ne = std::make_unique<QuadTree>(level + 1, FRect(x + w, y, w, h));
        sw = std::make_unique<QuadTree>(level + 1, FRect(x, y + h, w, h));
        se = std::make_unique<QuadTree>(level + 1, FRect(x + w, y + h, w, h));
    }

public:
    QuadTree(int lvl, const FRect& rect) : level(lvl), is_infinite(false), boundary(rect)
    {
    }

    void Insert(SPhysicsBaseUtility* obj)
    {
        auto rect = obj->GetCollisionBox();
        if (!boundary.contains(rect) && !boundary.intersects(rect))
            return;

        if (objects.size() < MAX_OBJECTS || level >= MAX_LEVELS)
        {
            objects.push_back(obj);
            return;
        }

        if (!nw) Subdivide();

        nw->Insert(obj);
        ne->Insert(obj);
        sw->Insert(obj);
        se->Insert(obj);
    }

    // 查询与 range 相交的对象
    // void query(const FRect& range, std::vector<Actor*>& found) const
    // {
    //     if (!boundary.intersects(range)) return;
    //
    //     for (auto& [objRect, obj] : objects)
    //     {
    //         if (range.intersects(objRect)) found.push_back(obj);
    //     }
    //
    //     if (!nw) return;
    //
    //     nw->query(range, found);
    //     ne->query(range, found);
    //     sw->query(range, found);
    //     se->query(range, found);
    // }
    void Query()
    {
        size_t n = objects.size();
        for (int i{}; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (objects[i]->GetCollisionBox().intersects(objects[j]->GetCollisionBox()))
                {
                   collision_callback(objects[i],objects[j]);
                }
            }
        }
    }


    // 清空四叉树
    void clear()
    {
        objects.clear();
        if (nw) { nw->clear(); nw.reset(); }
        if (ne) { ne->clear(); ne.reset(); }
        if (sw) { sw->clear(); sw.reset(); }
        if (se) { se->clear(); se.reset(); }
    }
};

#endif