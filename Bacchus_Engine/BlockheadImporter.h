#ifndef __BLOCKHEAD_IMPORTER_H__
#define __BLOCKHEAD_IMPORTER_H__

#include "Blockhead.h"
#include "Math.h"

class GameObject;
class ComponentTransform;

class BlockheadImporter : public Blockhead
{
public:

	BlockheadImporter(char* name);
	~BlockheadImporter();

	bool Draw();


	bool TestImport();
	
	
	/*GameObject* importObject = nullptr;*/
	float scale = 1.0f;
	/*ComponentTransform* importTransform = nullptr;
	float3 importScale;*/
	float3 importScale;
};

#endif
