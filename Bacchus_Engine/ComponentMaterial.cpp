#include "ComponentMaterial.h"

#include "mmgr/mmgr.h"


ComponentMaterial::ComponentMaterial(Component::ComponentType type) : Component(type)
{
}

ComponentMaterial::~ComponentMaterial()
{
	resource_material->resource_diffuse->instances--;
}

void ComponentMaterial::FreeTexture()
{
	resource_material->resource_diffuse->FreeMemory();
}

