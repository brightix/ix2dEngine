#pragma once
#include <atomic>
#include <condition_variable>
#include <queue>

#include "Classes/Object.hpp"
#include "../Component/SenceComponent/Texture.hpp"
#include "Classes/SubSystem/Sub/EngineSubsystem.hpp"
#include "Classes/Widget/PanelWidget/PanelWidget.hpp"
#include "Types/RenderData.hpp"
#include "Types/Enums/RenderPriority.hpp"

class Actor;
struct Transform;

//需要排序

class RendererCenter : public EngineSubSystem
{
	static SDL_Renderer* renderer;
public:
	std::shared_ptr<SDL_Texture> DefaultTexture;
	SDL_Window* window;
    RendererCenter();
	void Init() override;
	void DeInit() override;

	SDL_Renderer* GetRenderer() const;

	//收到新数据我就渲染上一帧
	void RenderScene(std::vector<RenderData>& clips);

	void RenderWidget(GCWeakPtr<PanelWidget> viewport);

	static SDL_Texture* CreateOutLineTexture(const Vec2<float>& size, SDL_Color color = {255,0,0,255});

	static SDL_Texture* CreateFilledTexture(Vec2<int> size);
	// TODO 添加AsyncSetTexture();
	static void SetTextureFromSurface(Texture* t, std::shared_ptr<SDL_Surface> new_surface);
	static void AsyncGetTextureFromSurface(GCWeakPtr<Texture> owner, std::shared_ptr<SDL_Surface> new_surface);

	void InitSDL();

	void DeInitSDL() const;
	std::shared_ptr<SDL_Texture> GetDefaultTexture();
};

