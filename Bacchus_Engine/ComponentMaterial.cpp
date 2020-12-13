#include "ComponentMaterial.h"

#include "mmgr/mmgr.h"


ComponentMaterial::ComponentMaterial(Component::ComponentType type) : Component(type)
{
}

ComponentMaterial::~ComponentMaterial()
{
	if (resource_material->resource_diffuse)
		resource_material->resource_diffuse->instances--;
}

void ComponentMaterial::FreeTexture()
{
	if (resource_material->resource_diffuse)
		resource_material->resource_diffuse->FreeMemory();
}

