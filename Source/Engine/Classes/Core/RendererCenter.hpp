#pragma once
#include <atomic>
#include <condition_variable>
#include <queue>

#include "Classes/Object.hpp"
#include "../Component/SenceComponent/Texture.hpp"
#include "Classes/SubSystem/Sub/EngineSubsystem.hpp"
#include "Types/RenderData.hpp"
#include "Types/Enums/RenderPriority.hpp"

class Actor;
struct Transform;


struct ActorShot
{
	Actor* actor;
	int layer_id;

	std::vector<Texture> textures;//相对actor内部的 层级 已假定有序

};

//需要排序

struct RenderTask
{
	RenderPriority priority;
	std::function<EventParams()> task;
	std::function<void(EventParams)> callback;
	std::string task_name;

	std::shared_ptr<SDL_Texture> assist_texture;
	RenderTask() : priority(TakeItEasy) { }

	RenderTask(const std::function<EventParams()>& t, RenderPriority p = TakeItEasy, std::function<void()> call_back = {}) : priority(p)
	{
		task = t;
	}
	// void SetCallback(const std::function<void(EventParams)>& call_back)
	// {
	// 	callback = call_back;
	// }
	// void SetTask(const std::function<void()>& t)
	// {
	// 	task = t;
	// }
	void SetPriority(RenderPriority p)
	{
		priority = p;
	}
	auto operator()() const
	{
		return task();
	}
};

inline std::mutex RendererTask_MTX;

inline auto cmp = [](const RenderTask& A, const RenderTask& B) {return A.priority < B.priority; };
static std::priority_queue<RenderTask,std::vector<RenderTask>, decltype(cmp)> priority_task{cmp};

class RendererCenter : public EngineSubSystem
{

	std::vector<RenderData> render_list[2];
	std::atomic<bool> render_fence[2];
	static std::atomic<bool> is_stop;
	static std::condition_variable render_cv;
	std::atomic<int> render_idx = 0;
	std::mutex render_mtx;
	std::thread render_thread;

	static SDL_Renderer* renderer;
	SDL_Window* window;

	static bool is_multithreading;

	std::mutex left_callback_mtx;
	std::queue<std::pair<std::function<void(EventParams)>,EventParams>> left_callback;
public:
	std::shared_ptr<SDL_Texture> DefaultTexture;
    RendererCenter();
	void Init() override;
	void DeInit() override;
	void StartRenderThread();
	void PushRenderData(const std::vector<RenderData>& render_data);
	static void AddRendererTask(RenderTask task);

	SDL_Renderer* GetRenderer() const;

	void Quit();

	 ~RendererCenter();

	//收到新数据我就渲染上一帧
	void RenderScene(std::vector<RenderData> &clips);

	void RenderUMG(std::unordered_set<GCPtr<Widget>> *clips);

	static std::shared_ptr<SDL_Texture> CreateOutLineTexture(const FRect& rect);

	static SDL_Texture* CreateFilledTexture(const FRect& rect);
	// TODO 添加AsyncSetTexture();
	static void SetTextureFromSurface(Texture *t, std::shared_ptr<SDL_Surface> new_surface);
	static void AsyncGetTextureFromSurface(GCWeakPtr<Texture> owner, std::shared_ptr<SDL_Surface> new_surface);
	void ReadLeftCallback();

	void InitSDL();

	void DeInitSDL() const;
};

