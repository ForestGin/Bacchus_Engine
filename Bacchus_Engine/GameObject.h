#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include "Globals.h"
#include "Res.h"
#include <list>
#include <string>
#include "Math.h"

class ResourceMaterial;

class GameObject
{

public:

	GameObject(const char* name);
	virtual ~GameObject();

	
	uint			GetUID() const;
	float3			GetPosition();
	float4x4        GetLocalTransform();
	Res*			GetResource(Res::ResType type);
	std::string		GetName() const;

	
	Res* AddResource(Res::ResType type);

	
	void			SetPosition(float x, float y, float z);
	void			SetRotationAxisAngle(const float3& rot_axis, float degrees_angle);
	void			Scale(float x, float y, float z);
	void			SetLocalTransform(float4x4 new_transform);
	void			SetMaterial(ResourceMaterial* material);


private:
	
	uint UID = 0;
	float4x4 Local_transform = math::float4x4::identity;
	std::string name;
	std::list<Res*> components;
};

#endif
