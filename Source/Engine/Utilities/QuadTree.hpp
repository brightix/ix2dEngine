#pragma once
#include "Classes/Core/GameEngine.hpp"

#include <functional>
#include <memory>
#include <vector>

#include "FuncLib/Deleter.hpp"
#include "Types/FRect.hpp"

class SPhysicsBaseUtility;

class QuadTree
{
    static constexpr int MAX_OBJECTS = 4; // 每个节点最大存储对象数
    static constexpr int MAX_LEVELS = 5;  // 最大深度

    int level;
    bool is_infinite;
    FRect boundary;
    std::vector<SPhysicsBaseUtility*> objects;
    std::unique_ptr<QuadTree> tree_slots[4]; // 0:nw, 1:ne, 2:sw, 3:se
    std::function<void(SPhysicsBaseUtility*, SPhysicsBaseUtility*)> collision_callback;

public:
    QuadTree() : level(0), is_infinite(false) {}

    QuadTree(int lvl, const FRect& rect)
        : level(lvl), is_infinite(false), boundary(rect) {}

    ~QuadTree() = default;

    void SetCollisionStrategy(const std::function<void(SPhysicsBaseUtility*, SPhysicsBaseUtility*)>& callback)
    {
        collision_callback = callback;
    }

    // 分裂四个子节点
    void Subdivide()
    {
        float x = boundary.x;
        float y = boundary.y;
        float w = boundary.w / 2.f;
        float h = boundary.h / 2.f;

        tree_slots[0] = std::make_unique<QuadTree>(level + 1, FRect(x, y, w, h));           // nw
        tree_slots[1] = std::make_unique<QuadTree>(level + 1, FRect(x + w, y, w, h));       // ne
        tree_slots[2] = std::make_unique<QuadTree>(level + 1, FRect(x, y + h, w, h));       // sw
        tree_slots[3] = std::make_unique<QuadTree>(level + 1, FRect(x + w, y + h, w, h));   // se
    }

    void Insert(SPhysicsBaseUtility* obj)
    {
        auto rect = obj->GetCollisionBox();
        if (!boundary.contains(rect) && !boundary.intersects(rect))
            return;

        if (objects.size() < MAX_OBJECTS || level >= MAX_LEVELS)
        {
        	//std::cout << obj->test_name + "   插入  " + std::to_string(level) + " 层" << std::endl;
            objects.push_back(obj);
            return;
        }

        if (!tree_slots[0]) Subdivide();

        for (auto& child : tree_slots)
        {
            if (child)
                child->Insert(obj);
        }
    }

    // 检测当前节点的对象对
    void Query()
    {
        size_t n = objects.size();
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = i + 1; j < n; ++j)
            {
                if (objects[i]->GetCollisionBox().intersects(objects[j]->GetCollisionBox()))
                {
                    if (collision_callback)
                        collision_callback(objects[i], objects[j]);
                }
            }
        }

        // 向下递归
        for (auto& child : tree_slots)
        {
            if (child)
                child->Query();
        }
    }

    // 递归调试输出
    void DebugTree(int depth = 0)
    {
    	auto rect = boundary.CastToSDL_FRect();
		auto texture = TTexture(Create_OutLineTexture_S({boundary.w,boundary.h}));
    	SDL_RenderTexture(GetRenderer(), texture.get(), nullptr, &rect);
    	if (tree_slots[0])
    	{
    		for (auto& child : tree_slots)
    		{
    			child->DebugTree(depth + 1);
    		}
    	}
    }
	void SetBoundary(const FRect& new_boundary)
    {
	    boundary = new_boundary;
    }
    // 清空四叉树
    void Clear()
    {
        objects.clear();
        for (auto& child : tree_slots)
        {
            if (child)
            {
                child->Clear();
                child.reset();
            }
        }
    }
};
// #pragma once
//
// #if 1
// // 如果定义了 DISABLE_THIS_FILE，就不编译下面内容
// #include <functional>
// #include <memory>
//
// #include "Types/FRect.hpp"
// #include <vector>
//
// #include "Classes/Core/GameEngine.hpp"
// #include "Classes/Core/RendererCenter.hpp"
//
// class QuadTree
// {
//     static constexpr int MAX_OBJECTS = 4; // 每个节点最大存储对象数
//     static constexpr int MAX_LEVELS = 5;  // 最大深度
//
//     int level;
//     bool is_infinite;
//     FRect boundary;
//     std::vector<SPhysicsBaseUtility*> objects;
//     std::unique_ptr<QuadTree> nw, ne, sw, se;
// 	std::unique_ptr<QuadTree> tree_slots[4];
//     std::function<void(SPhysicsBaseUtility*,SPhysicsBaseUtility*)> collision_callback;
// public:
//     QuadTree() : level(0), is_infinite(false)
//     {
//     }
//
//     void SetCollisionStrategy(const std::function<void(SPhysicsBaseUtility*,SPhysicsBaseUtility*)>& callback)
//     {
//         collision_callback = callback;
//     }
//     ~QuadTree(){}
//     // 分裂四个子节点
//     void Subdivide()
//     {
//         float x = boundary.x;
//         float y = boundary.y;
//         float w = boundary.w / 2.f;
//         float h = boundary.h / 2.f;
//
//         nw = std::make_unique<QuadTree>(level + 1, FRect(x, y, w, h));
//         ne = std::make_unique<QuadTree>(level + 1, FRect(x + w, y, w, h));
//         sw = std::make_unique<QuadTree>(level + 1, FRect(x, y + h, w, h));
//         se = std::make_unique<QuadTree>(level + 1, FRect(x + w, y + h, w, h));
//     }
//
// public:
//     QuadTree(int lvl, const FRect& rect) : level(lvl), is_infinite(false), boundary(rect)
//     {
//     }
//
//     void Insert(SPhysicsBaseUtility* obj)
//     {
//         auto rect = obj->GetCollisionBox();
//         if (!boundary.contains(rect) && !boundary.intersects(rect))
//             return;
//
//         if (objects.size() < MAX_OBJECTS || level >= MAX_LEVELS)
//         {
//             objects.push_back(obj);
//             return;
//         }
//
//         if (!nw) Subdivide();
//
//         nw->Insert(obj);
//         ne->Insert(obj);
//         sw->Insert(obj);
//         se->Insert(obj);
//     }
//
//     // 查询与 range 相交的对象
//     // void query(const FRect& range, std::vector<Actor*>& found) const
//     // {
//     //     if (!boundary.intersects(range)) return;
//     //
//     //     for (auto& [objRect, obj] : objects)
//     //     {
//     //         if (range.intersects(objRect)) found.push_back(obj);
//     //     }
//     //
//     //     if (!nw) return;
//     //
//     //     nw->query(range, found);
//     //     ne->query(range, found);
//     //     sw->query(range, found);
//     //     se->query(range, found);
//     // }
//     void Query()
//     {
//         size_t n = objects.size();
//         for (int i{}; i < n; ++i)
//         {
//             for (int j = i + 1; j < n; ++j)
//             {
//                 if (objects[i]->GetCollisionBox().intersects(objects[j]->GetCollisionBox()))
//                 {
//                    collision_callback(objects[i],objects[j]);
//                 }
//             }
//         }
//     }
//
//     void DebugTree(int depth = 0)
//     {
//         // std::string indent(depth * 2, ' ');
//         // printf("%sLevel %d, Objects: %zu, Bounds: (%.1f, %.1f, %.1f, %.1f)\n",
//         //        indent.c_str(), level, objects.size(),
//         //        boundary.x, boundary.y, boundary.w, boundary.h);
//
// 	    auto rect = boundary.CastToSDL_FRect();
//     	//auto texture = RendererCenter::CreateOutLineTexture(boundary);
//     	auto t = TTexture(RendererCenter::CreateOutLineTexture({boundary.x,boundary.y}));
//     	SDL_RenderTexture(GetRenderer(), t.get(),nullptr,&rect);
// 	    if (nw)
// 	    {
// 	    	nw->DebugTree();
// 	    }
//     }
//

//     // 清空四叉树
//     void Clear()
//     {
//         objects.clear();
//         if (nw) { nw->Clear(); nw.reset(); }
//         if (ne) { ne->Clear(); ne.reset(); }
//         if (sw) { sw->Clear(); sw.reset(); }
//         if (se) { se->Clear(); se.reset(); }
//     }
// };
//
// #endif