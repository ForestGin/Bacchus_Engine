#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Res.h"
#include <list>
#include <string>

class GameObject
{

public:

	GameObject(const char* name);
	virtual ~GameObject();

	// --- Getters ---
	uint GetUID() const;
	Res* AddResource(Res::ResType type);

private:
	// Unique Identifier
	uint UID = 0;

public:
	std::list<Res*> components;
	std::string name;
};

#endif
