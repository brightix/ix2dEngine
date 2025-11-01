#pragma once
#include "SceneComponent.hpp"


class SceneTextBlock : public SceneComponent
{
	std::string text;
public:
    SceneTextBlock();
	void Construct() override;

	void SetText(const std::string& new_text);
};

