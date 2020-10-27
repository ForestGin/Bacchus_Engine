#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Res.h"
#include "Math.h"



struct aiMesh;


class ResourceMesh : public Res
{
public:

	ResourceMesh(GameObject* ContainerGO);
	virtual ~ResourceMesh();

	void ImportMesh(const aiMesh* mesh);

public:

	float3* Vertices = nullptr;
	uint VerticesID = 0; // unique vertex in VRAM
	uint VerticesSize = 0;

	uint* Indices = nullptr;
	uint IndicesID = 0; // index in VRAM
	uint IndicesSize = 0;

	float3* Normals = nullptr;
	uint NormalsSize = 0;

	float* TexCoords = nullptr;
	uint  TextureCoordsID = 0;
	uint TexCoordsSize = 0;
	

	

};

#endif

