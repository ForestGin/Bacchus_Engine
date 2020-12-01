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

private:

	bool TestImport();
	
	GameObject* importObject = nullptr;
	/*ComponentTransform* importTransform = nullptr;
	float3 importScale;*/
};

#endif
