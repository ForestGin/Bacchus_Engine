#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include "Globals.h"

class Resource
{
	friend class ModuleResources; //it can access private and protected

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
	virtual ~Resource();

	uint GetUID() const;
	void SetUID(uint UID);

	ResourceType GetType() const;

	const char* GetName() const;

	void SetName(const char* name);
	void SetOriginalFilename(const char* filename);

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
