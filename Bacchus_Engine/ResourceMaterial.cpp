#include "ResourceMaterial.h"
#include "OpenGL.h"

#include "mmgr/mmgr.h"


ResourceMaterial::ResourceMaterial(Res::ResType type) : Res(type)
{
}

ResourceMaterial::~ResourceMaterial()
{
	glDeleteBuffers(1, (GLuint*)&TextureID);
}

