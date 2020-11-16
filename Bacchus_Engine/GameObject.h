#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Res.h"
#include <vector>
#include <string>
#include "Math.h"

class ResourceMaterial;

class GameObject
{

public:

	GameObject(const char* name);
	virtual ~GameObject();
	void Enable();
	void Disable();
	void Update(float dt);
	
	uint& GetUID();
	std::string	GetName() const;
	
	bool& GetActive();
	bool IsEnabled() const;

	template<typename TRes>
	TRes* GetResource(Res::ResType type)
	{

		for (uint i = 0; i < components.size(); ++i)
		{
			if (components[i]->GetType() == type)
			{
				return ((TRes*)(components[i]));
			}
		}

		return nullptr;
	}

	
	Res* AddResource(Res::ResType type);
	void RemoveResource(Res::ResType type);
	bool HasResource(Res::ResType type) const;
	std::vector<Res*>& GetResources();

	void SetMaterial(ResourceMaterial* material);
	void SetName(const char* name);

	void AddChildGO(GameObject* GO);
	bool FindChildGO(GameObject* GO);
	void RecursiveDelete(GameObject* GO);
	void RemoveChildGO(GameObject* GO);
	void OnUpdateTransform(GameObject* GO);


public:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
private:
	
	uint UID = 0;
	std::string name;
	std::vector<Res*> components;
	bool active = false;
};

#endif
