#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Component.h"
#include <vector>
#include <string>
#include "Math.h"

class ComponentMaterial;

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

	template<typename TComponent>
	TComponent* GetComponent(Component::ComponentType type)
	{

		for (uint i = 0; i < components.size(); ++i)
		{
			if (components[i]->GetType() == type)
			{
				return ((TComponent*)(components[i]));
			}
		}

		return nullptr;
	}

	
	Component* AddComponent(Component::ComponentType type);
	void RemoveComponent(Component::ComponentType type);
	bool HasComponent(Component::ComponentType type) const;
	std::vector<Component*>& GetComponents();

	void SetMaterial(ComponentMaterial* material);
	void SetName(const char* name);

	void AddChildGO(GameObject* GO);
	bool FindChildGO(GameObject* GO);
	void RemoveChildGO(GameObject* GO);

	void RecursiveDelete(bool target = true);
	void OnUpdateTransform();

	const AABB& GetAABB() const;
	const OBB& GetOBB() const;

	void UpdateAABB();

public:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;
private:
	
	uint UID = 0;
	std::string name;
	std::vector<Component*> components;
	bool active = false;

	AABB aabb;
	OBB obb;
};

#endif
