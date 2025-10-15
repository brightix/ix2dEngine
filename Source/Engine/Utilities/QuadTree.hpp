#pragma once
#include "Types/FRect.hpp"


class QuadTree
{
    static constexpr int MAX_OBJECTS = 4; // 每个节点最大存储对象数
    static constexpr int MAX_LEVELS = 5;  // 最大深度

    int level;
    bool is_infinite;
    FRect boundary;
    std::vector<SPhysicsUtilityBase*> objects;
    std::unique_ptr<QuadTree> nw, ne, sw, se;
public:
    ~QuadTree();


    // 分裂四个子节点
    void subdivide()
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

    void insert(const Actor* obj)
    {
        rect = obj->GetCollisionBox();
        if (!boundary.contains(rect) && !boundary.intersects(rect))
            return;

        if (objects.size() < MAX_OBJECTS || level >= MAX_LEVELS)
        {
            objects.emplace_back(rect, obj);
            return;
        }

        if (!nw) subdivide();

        nw->insert(rect, obj);
        ne->insert(rect, obj);
        sw->insert(rect, obj);
        se->insert(rect, obj);
    }

    // 查询与 range 相交的对象
    void query(const FRect& range, std::vector<Actor*>& found) const
    {
        if (!boundary.intersects(range)) return;

        for (auto& [objRect, obj] : objects)
        {
            if (range.intersects(objRect)) found.push_back(obj);
        }

        if (!nw) return;

        nw->query(range, found);
        ne->query(range, found);
        sw->query(range, found);
        se->query(range, found);
    }
    void query()
    {
        int n = objects.size();
        for (int i{}; i < n; ++i)
        {
            for (int j = i + 1; j < n; ++j)
            {
                if (objects[i]->GetCollisionBox().intersects(objects[j]))
                {
                    SPhysics::OnRigidCollision(Objects[i]);
                }

            }
        }




        for (auto& send : objects)//碰撞发起者
            //std::unordered_map<Actor*,Actor*>
            std::unordered_set<Actor*> send_collision_map;
            for (auto& recv : objects)//碰撞接受者
            {
                std::unordered_set<Actor*> recv_collision_set;
                if (send == recv || send_collision_map.contains(recv)) return;
                std::unordered_map<Actor*,Actor*> recv_collision_map;
                for (FRect*& rect : send->GetCollisionBoxGroup())//发起者的所有碰撞体{
                {

                }
                if (send->GetCollisionBox)
            }
            boundary.intersects(obj->RenderCollisionBox())
            {

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
