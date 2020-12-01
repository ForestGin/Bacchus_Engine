#ifndef __BLOCKHEAD_INSPECTOR_H__
#define __BLOCKHEAD_INSPECTOR_H__

#include "Blockhead.h"
#include "Math.h"

class GameObject;
class ComponentTransform;


class BlockheadInspector : public Blockhead
{
public:

	BlockheadInspector(char* name);
	~BlockheadInspector();

	bool Draw();

	bool Startup = true;

	GameObject* Selected = nullptr;
	ComponentTransform* transform = nullptr;
	float3 scale;
};

#endif
