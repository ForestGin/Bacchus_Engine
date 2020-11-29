#include "ComponentMesh.h"

#include "OpenGL.h"
#include "Globals.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

ComponentMesh::ComponentMesh(GameObject* ContainerGO) : Component(ContainerGO, Component::ComponentType::Mesh)
{

}

ComponentMesh::~ComponentMesh()
{
	
	
}


