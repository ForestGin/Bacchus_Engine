#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include "Globals.h"

class Resource
{
public:
	enum class ResourceType
	{
		FOLDER,
		MESH,
		TEXTURE,
		MATERIAL,
		METAFILE,

		UNKNOWN,
	};

	Resource(ResourceType type);
	~Resource();

	uint GetUID() const;

	ResourceType GetType() const;

	const char* GetName() const;

	const char* GetOriginalFile() const;
	const char* GetResourceFile() const;
	

	virtual void LoadOnMemory() {};
	virtual void FreeMemory() {};

public:
	uint instances = 0;

protected:
	uint UID = 0;

	ResourceType type = ResourceType::UNKNOWN;

	std::string name = "";

	std::string og_file = "";
	std::string res_file = "";
};

#endif // !__RESOURCE_H__
