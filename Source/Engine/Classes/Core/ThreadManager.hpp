#pragma once
#include <atomic>
#include <condition_variable>

#include "Classes/Object.hpp"
#include "../Component/SenceComponent/Texture.hpp"
#include "Types/Transform.hpp"


class Actor;
struct Transform;
struct StaticTexture;


struct ActorShot
{
	Actor* actor;
	int layer_id;

	std::vector<Texture> textures;//相对actor内部的 层级 已假定有序
};

//需要排序
struct RenderBufferShot
{
	std::vector<Actor*> actors;
	std::vector<Transform> transforms;
	std::vector<StaticTexture> sprites;
};


class ThreadManager : public Object
{
	RenderBufferShot render_buffer_shot[2];
	std::atomic<bool> render_fence[2];
	std::atomic<bool> is_stop;
	std::condition_variable render_cv;
	int render_idx = 0;
	std::mutex render_mtx;
public:
    ThreadManager();

	void StartRenderThread();
    ~ThreadManager()= default;
};

