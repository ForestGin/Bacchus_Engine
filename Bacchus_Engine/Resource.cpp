#include "Resource.h"

Resource::Resource(ResourceType type)
{
	this->type = type;
}

Resource::~Resource()
{
}

uint Resource::GetUID() const
{
	return UID;
}

Resource::ResourceType Resource::GetType() const
{
	return type;
}

const char* Resource::GetName() const
{
	return name.data();
}

const char* Resource::GetOriginalFile() const
{
	return og_file.data();
}

const char* Resource::GetResourceFile() const
{
	return res_file.data();
}
