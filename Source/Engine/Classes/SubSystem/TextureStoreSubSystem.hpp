#pragma once
#include <SDL3/SDL_render.h>
#include "Sub/EngineSubsystem.hpp"
#include "Utilities/DataStructure/LRUCache.hpp"


class TextureStoreSubSystem : public EngineSubSystem
{
	LRUCache<std::string, SDL_Texture*> cached_texture;
	std::shared_ptr<SDL_Texture> DefaultTexture;
public:
    TextureStoreSubSystem();
	void Init() override;
	SDL_Texture* GetTexture(const std::string& texture_name);

	SDL_Texture* LoadInCache(const std::string& texture_name);
	void DeInit() override;
	std::shared_ptr<SDL_Texture> GetDefaultTexture();
};

