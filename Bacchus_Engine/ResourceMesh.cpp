#include "ResourceMesh.h"

#include "OpenGL.h"
#include "Globals.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

ResourceMesh::ResourceMesh(GameObject* ContainerGO) : Res(ContainerGO, Res::ResType::Mesh)
{

}

ResourceMesh::~ResourceMesh()
{
	glDeleteBuffers(1, (GLuint*)&VerticesID);
	glDeleteBuffers(1, (GLuint*)&IndicesID);
	glDeleteBuffers(1, (GLuint*)&TextureCoordsID);
	

	if (Vertices)
	{
		delete[] Vertices;
		Vertices = nullptr;
	}
	if (Indices)
	{
		delete[] Indices;
		Indices = nullptr;
	}
	if (Normals)
	{
		delete[] Normals;
		Normals = nullptr;
	}
	if (TexCoords)
	{
		delete[] TexCoords;
		TexCoords = nullptr;
	}
	
}


