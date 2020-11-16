#include "Res.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

Res::Res(GameObject* ContainerGO, Res::ResType type)
{
	GO = ContainerGO;
	this->type = type;
	Enable();
}

Res::Res(Res::ResType type)
{
	this->type = type;
	Enable();
}

Res::~Res()
{
	Disable();
}

void Res::Enable()
{
	active = true;
}

void Res::Disable()
{
	active = false;
}

bool Res::IsEnabled() const
{
	return active;
}

bool& Res::GetActive()
{
	return active;
}


Res::ResType Res::GetType() const
{
	return type;
}

GameObject* Res::GetContainerGameObject() const
{
	return GO;
}