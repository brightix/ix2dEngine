#pragma once
#include <string>

template<typename T>
concept HasStr = requires(T t)
{
	{ t.Str() } ->std::same_as<std::string>;
};

template<typename Derived>
struct VarTypeCheck {
	VarTypeCheck() {
		static_assert(HasStr<Derived>, "Derived must implement Str()");
	}
};