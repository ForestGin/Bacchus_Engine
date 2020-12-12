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
	void SetFlipped();
	void SetCamIgnored();
	void SetXAxis();
	void SetZAxis();
		
	
	float scale = 1.0f;
	float axis = 0.0f;
	float3 importScale;
	float3 importRotation;
};

#endif
