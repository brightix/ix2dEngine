#include "Utilities/GCPtr.hpp"
#include "Classes/Object.hpp"

void GCLink(Object* child, Object* parent)
{
	if (!child || !parent)
	{
		//Log("GCLink 绑定到空指针");
		//std::cout << "绑定到空指针" << std::endl;
		return ;
	}
	child->referenced.push_back(parent);
	parent->referencing.push_back(child);
}

void GCUnLink(Object* child, Object* parent)
{
	if (!child || !parent)
	{
		//Log("GCLink 绑定到空指针");
		//std::cout << "接触到空指针" << std::endl;
		return ;
	}
	// 从 parent->referencing 移除 child
	auto& refs = parent->referencing;
	std::erase(refs, child);

	// 从 child->referenced 移除 parent
	auto& parents = child->referenced;
	std::erase(parents, parent);
}
