#ifndef __RES_H__
#define __RES_H__

#include "Globals.h"

class GameObject;

class Res
{
public:
	enum class ResType
	{
		Transform,
		Mesh,
		Renderer,
		Material,
		Unknown
	};

public:

	Res(GameObject* ContainerGO, Res::ResType type);
	Res(Res::ResType type);
	virtual ~Res();
	virtual void Enable();
	virtual void Disable();

	ResType GetType() const;
	GameObject* GetContainerGameObject() const;
	bool& GetActive();
	bool IsEnabled() const;

protected:
	GameObject* GO = nullptr;
	ResType type = ResType::Unknown;
	bool active = false;

};

#endif
