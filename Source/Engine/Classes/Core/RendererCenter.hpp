#pragma once
#include <condition_variable>

#include "Classes/Object.hpp"
#include "Classes/SubSystem/Sub/EngineSubsystem.hpp"
#include "Classes/Widget/PanelWidget/PanelWidget.hpp"
#include "Types/RenderData.hpp"

//需要排序

class Texture;

class RendererCenter : public EngineSubSystem
{
	SDL_Renderer* renderer;
public:
	std::shared_ptr<SDL_Texture> DefaultTexture;
	SDL_Window* window;
    RendererCenter();
	void Init() override;
	void DeInit() override;

	[[nodiscard]] SDL_Renderer* GetRenderer() const;

	void SetRendererAndWindow(SDL_Renderer* r, SDL_Window* w);


	//收到新数据我就渲染上一帧
	void RenderScene(std::vector<RenderData>& clips);

	void RenderWidget(GCPtr<PanelWidget> viewport);

	std::shared_ptr<SDL_Texture> CreateOutLineTexture(const Vec2<float>& size, SDL_Color color = RED);

	std::shared_ptr<SDL_Texture> CreateFilledTexture(Vec2<float> size, SDL_Color color = EmeraldGreen);
	// TODO 添加AsyncSetTexture();
	void SetTextureFromSurface(Texture* t, std::shared_ptr<SDL_Surface> new_surface);
	//static void AsyncGetTextureFromSurface(GCPtr<Texture> owner, std::shared_ptr<SDL_Surface> new_surface);

	void InitSDL();

	void DeInitSDL() const;
	std::shared_ptr<SDL_Texture> GetDefaultTexture();
};

void SetTextureFromSurface_S(Texture* t, std::shared_ptr<SDL_Surface> new_surface);


