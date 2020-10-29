#include "ResourceMaterial.h"
#include "OpenGL.h"

#include "mmgr/mmgr.h"


ResourceMaterial::ResourceMaterial(Res::ResType type) : Res(type)
{
}

ResourceMaterial::~ResourceMaterial()
{
	glDeleteTextures(1, (GLuint*)&TextureID);
}

void ResourceMaterial::FreeTexture()
{
	glDeleteTextures(1, (GLuint*)&TextureID);
}

