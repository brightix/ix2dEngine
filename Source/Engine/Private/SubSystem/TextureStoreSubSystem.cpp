#include "Classes/SubSystem/TextureStoreSubSystem.hpp"


#include <SDL_image.h>
#include "Classes/Core/GameEngine.hpp"
#include "Utilities/FuncLib/Deleter.hpp"

TextureStoreSubSystem::TextureStoreSubSystem()
{
	name = "TextureStoreSubSystem";
	cached_texture.OnEvict = [](SDL_Texture* t) {
		SDL_DestroyTexture(t);
	};
}

void TextureStoreSubSystem::Init()
{
	// auto task = RenderTask();
	// task.task = []() {
	// 	EventParams e;
	// 	e.Add("new_texture",RendererCenter::CreateOutLineTexture(FRect(0,0,10,10)));
	// 	return e;
	// };
	// task.callback = [this](EventParams e) {
	// 	DefaultTexture = *e.Get<std::shared_ptr<SDL_Texture>>("new_texture");
	// };
	DefaultTexture = GameEngine::Instance().GetDefaultTexture();
}

SDL_Texture* TextureStoreSubSystem::GetTexture(const std::string& texture_name)
{
	if (auto ret = cached_texture.get(texture_name))
	{
		return *ret;
	}
	return LoadInCache(texture_name);
}

SDL_Texture* TextureStoreSubSystem::LoadInCache(const std::string& texture_name)
{
	std::string file_path = "Content/Textures" + texture_name;
	SDL_Texture* texture = IMG_LoadTexture(GetRenderer(), file_path.c_str());
	if (texture)
	{
		cached_texture.put(texture_name, texture);
	}
	else
	{
		Log("未能找到名为\"" + texture_name + "\"的纹理");
	}
	return texture;
}

void TextureStoreSubSystem::DeInit()
{
	EngineSubSystem::DeInit();
	DefaultTexture.reset();
	cached_texture.clear();
}

