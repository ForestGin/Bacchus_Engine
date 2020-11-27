#include "Resource.h"
#include "Application.h"

#include "mmgr/mmgr.h"

Resource::Resource(ResourceType type)
{
	this->type = type;
	UID = App->GetRandom().Int();
}

Resource::~Resource()
{
}

uint Resource::GetUID() const
{
	return UID;
}

void Resource::SetUID(uint UID)
{
	this->UID = UID;
}

Resource::ResourceType Resource::GetType() const
{
	return type;
}

const char* Resource::GetName() const
{
	return name.data();
}

void Resource::SetName(const char* name)
{
	this->name = name;
}

void Resource::SetOriginalFilename(const char* filename)
{
	this->og_file = filename;
}

const char* Resource::GetOriginalFile() const
{
	return og_file.data();
}

const char* Resource::GetResourceFile() const
{
	return res_file.data();
}
