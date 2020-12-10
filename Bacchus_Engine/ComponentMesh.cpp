#include "ComponentMesh.h"

#include "Globals.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

ComponentMesh::ComponentMesh(GameObject* ContainerGO) : Component(ContainerGO, Component::ComponentType::Mesh)
{

}

ComponentMesh::~ComponentMesh()
{
	
	
}

const AABB& ComponentMesh::GetAABB() const
{
	return resource_mesh->aabb;
}


