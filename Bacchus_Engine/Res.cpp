#include "Res.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

Res::Res(GameObject* ContainerGO, Res::ResType type)
{
	GO = ContainerGO;
	this->type = type;
}

Res::Res(Res::ResType type)
{
	this->type = type;
}

Res::~Res()
{
}

Res::ResType Res::GetType()
{
	return type;
}

GameObject* Res::GetContainerGameObject()
{
	return GO;
}