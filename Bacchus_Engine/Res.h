#ifndef __RES_H__
#define __RES_H__

#include "Globals.h"

class GameObject;

class Res
{
public:
	enum class ResType
	{
		Unknown = 0,
		Mesh
	};

public:

	Res(GameObject* ContainerGO, Res::ResType type);
	virtual ~Res();

private:
	GameObject* GO = nullptr;
	Res::ResType type;

};

#endif
