#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Res.h"

struct aiScene;

class ResourceMaterial : public Res
{
public:

	ResourceMaterial(Res::ResType type);
	virtual ~ResourceMaterial();

public:
	uint  TextureID = 0;

};

#endif 
