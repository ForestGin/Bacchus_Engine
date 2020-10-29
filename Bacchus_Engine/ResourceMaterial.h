#ifndef __RESOURCE_MATERIAL_H__
#define __RESOURCE_MATERIAL_H__

#include "Res.h"
#include <string>

struct aiScene;

class ResourceMaterial : public Res
{
public:

	ResourceMaterial(Res::ResType type);
	virtual ~ResourceMaterial();

	void FreeTexture();

public:
	uint  TextureID = 0;
	std::string Texture_path;
	uint Texture_width = 0;
	uint Texture_height = 0;

};

#endif 
