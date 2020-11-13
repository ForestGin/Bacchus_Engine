#ifndef __BLOCKHEAD_HIERARCHY_H__
#define __BLOCKHEAD_HIERARCHY_H__

#include "Blockhead.h"

class GameObject;

class BlockheadHierarchy : public Blockhead
{
public:

	BlockheadHierarchy(char* name);
	~BlockheadHierarchy();

	bool Draw();

private:

	void DrawRecursive(GameObject* Go);

private:

	GameObject* to_destroy = nullptr;
	bool end_drag = false;
	GameObject* target = nullptr;
	GameObject* dragged = nullptr;
};

#endif