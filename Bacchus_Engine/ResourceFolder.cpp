#include "ResourceFolder.h"

ResourceFolder::ResourceFolder() : Resource(Resource::ResourceType::FOLDER)
{
}

ResourceFolder::~ResourceFolder()
{
}

std::vector<Resource*> ResourceFolder::GetChilds() const
{
	return childs;
}