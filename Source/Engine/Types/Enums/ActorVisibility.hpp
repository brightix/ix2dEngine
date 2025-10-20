#pragma once


enum class ComponentVisibility : int
{
	Visible,		//可视  带	碰撞
	OnlyDraw,		//可视  不带	碰撞
	Hidden,			//隐藏  带	碰撞
	Fold			//隐藏  不带	碰撞
};
