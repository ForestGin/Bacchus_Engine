#ifndef __BLOCKHEAD_INSPECTOR_H__
#define __BLOCKHEAD_INSPECTOR_H__

#include "Blockhead.h"

class BlockheadInspector : public Blockhead
{
public:

	BlockheadInspector(char* name);
	~BlockheadInspector();

	bool Draw();

	bool Startup = true;

private:
	inline void CreateGameObjectNode(GameObject& Selected) const;
	inline void CreateTransformNode(GameObject& Selected) const;
	inline void CreateMeshNode(GameObject& Selected) const;
	inline void CreateRendererNode(GameObject& Selected) const;
	inline void CreateMaterialNode(GameObject& Selected) const;
	inline void CreateCameraNode(GameObject& Selected) const;
};

#endif
