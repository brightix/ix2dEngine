#pragma once
#include "Classes/Core/GameEngine.hpp"

#include <functional>
#include <memory>
#include <vector>

#include "FuncLib/Deleter.hpp"
#include "Types/FRect.hpp"

class SPhysicsBaseUtility;
using TCollisionPairs = std::unordered_map<SPhysicsBaseUtility*, std::unordered_set<SPhysicsBaseUtility*>>;
struct SymmetricPairHash
{
	std::size_t operator()(const std::pair<SPhysicsBaseUtility*, SPhysicsBaseUtility*>& p) const noexcept
	{
		auto a = min(p.first, p.second);
		auto b = max(p.first, p.second);
		return reinterpret_cast<std::uintptr_t>(a) * 1315423911u
			 ^ reinterpret_cast<std::uintptr_t>(b);
	}
};

struct SymmetricPairEqual
{
	bool operator()(const std::pair<SPhysicsBaseUtility*, SPhysicsBaseUtility*>& a,
					const std::pair<SPhysicsBaseUtility*, SPhysicsBaseUtility*>& b) const noexcept
	{
		return (a.first == b.first && a.second == b.second) ||
			   (a.first == b.second && a.second == b.first);
	}
};
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

    	// 如果不在当前边界范围内，直接返回
    	if (!boundary.intersects(rect))
    		return;

    	// 如果已细分，尝试插入到子节点
    	if (tree_slots[0])
    	{
    		for (auto& child : tree_slots)
    		{
    			if (child->boundary.contains(rect))
    			{
    				child->Insert(obj);
    				return; // 放进子节点后，不再留在父节点
    			}
    		}
    	}

    	// 放在当前节点
    	objects.push_back(obj);

    	// 如果超出容量并且可以继续分裂
    	if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
    	{
    		// 如果还没细分，先细分
    		if (!tree_slots[0])
    			Subdivide();

    		// 再分配旧对象
    		auto it = objects.begin();
    		while (it != objects.end())
    		{
    			auto* o = *it;
    			auto r = o->GetCollisionBox();
    			bool moved = false;

    			for (auto& child : tree_slots)
    			{
    				if (child->boundary.contains(r))
    				{
    					child->Insert(o);
    					it = objects.erase(it);
    					moved = true;
    					break;
    				}
    			}

    			if (!moved)
    				++it; // 没找到合适子节点就留在当前节点
    		}
    	}
    }


    // 检测当前节点的对象对
	void CollectIntersectingObjects(const FRect& area, std::vector<SPhysicsBaseUtility*>& out)
    {
    	if (!boundary.intersects(area))
    		return; // 整个子树都与 area 不相交，剪枝

    	// 本节点的对象：只把与 area 实际相交的对象加入
    	for (auto* o : objects)
    	{
    		if (o->GetCollisionBox().intersects(area))
    			out.push_back(o);
    	}

    	// 递归到子节点（如果有）
    	if (tree_slots[0])
    	{
    		for (auto& child : tree_slots)
    		{
    			if (child) child->CollectIntersectingObjects(area, out);
    		}
    	}
	}
	TCollisionPairs Query(TCollisionPairs& collision_pairs, const int depth = 0)
	{
	    TStartF("Query" + std::to_string(depth));

	    // 1) 检测本节点内对象对（原有逻辑）
	    size_t n = objects.size();
	    for (size_t i = 0; i < n; ++i)
	    {
	        auto* A = objects[i];
	        auto ARect = A->GetCollisionBox();

	        for (size_t j = i + 1; j < n; ++j)
	        {
	            auto* B = objects[j];
	            auto BRect = B->GetCollisionBox();
		        //
	        	// if (B->type == PhysicsType::Movable)
	        	// {
	        	// 	BREAK
	        	// }
	            if (ARect.intersects(BRect))
	            {
	                if (A->type != PhysicsType::Static)
	                    collision_pairs[A].insert(B);
	                else if (B->type != PhysicsType::Static)
	                    collision_pairs[B].insert(A);
	            }
	        }
	    }

	    // 2) 检测本节点的对象 与 子树（所有子节点及其后代）里面的对象之间的对
	    //    对每个本节点对象，遍历与它边界相交的子节点，收集候选并检测
	    if (tree_slots[0])
	    {
	        for (size_t i = 0; i < n; ++i)
	        {
	            auto* A = objects[i];
	            auto ARect = A->GetCollisionBox();

	            for (auto& child : tree_slots)
	            {
	                if (!child) continue;
	                if (!child->boundary.intersects(ARect)) continue; // 剪枝：子节点边界不相交则无必要深入

	                // 从 child 子树中收集与 A 的包围盒相交的对象
	                std::vector<SPhysicsBaseUtility*> candidates;
	                child->CollectIntersectingObjects(ARect, candidates);

	                for (auto* B : candidates)
	                {
	                    // 再次精确判断（虽然 Collect 已筛过，但保险起见）
	                    if (!A->GetCollisionBox().intersects(B->GetCollisionBox()))
	                        continue;

	                    // if (A->type == PhysicsType::Movable || B->type == PhysicsType::Movable)
	                    //     continue;

	                    if (A->type != PhysicsType::Static)
	                        collision_pairs[A].insert(B);
	                    else if (B->type != PhysicsType::Static)
	                        collision_pairs[B].insert(A);
	                }
	            }
	        }
	    }

	    // 3) 递归子节点自身查询（内部对子节点会自己处理）
	    for (auto& child : tree_slots)
	    {
	        if (child)
	        {
		        TStartF("child_query");
	        	child->Query(collision_pairs, depth + 1);
	        	TEndF("child_query");
	        }
	    }

	    TEndF("Query" + std::to_string(depth));
	    return collision_pairs;
	}

    // 递归调试输出
    void DebugTree(int depth = 0)
    {
    	auto rect = boundary.CastToSDL_FRect();
		auto texture = Create_OutLineTexture_S({boundary.w,boundary.h});
    	SDL_RenderTexture(GetRenderer(), texture.get(), nullptr, &rect);
    	for (auto& obj : objects)
    	{
    		auto loc = obj->collision_owner->GetComponentTransform().location;
    		auto size = obj->collision_owner->GetComponentSize();
    		SDL_FRect dst(loc.x,loc.y,size.x,size.y);
    		SDL_RenderTexture(GetRenderer(),Create_OutLineTexture_S({size.x,size.y}).get(),nullptr,&dst);
    	}
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