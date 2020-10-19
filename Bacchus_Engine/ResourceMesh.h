#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Globals.h"
#include "Res.h"


class ResourceMesh : public Res
{
public:

	ResourceMesh();
	virtual ~ResourceMesh();

	void Import(const char* path);

public:

	float* Vertices = nullptr;
	uint VerticesID = 0; // unique vertex in VRAM
	uint verticesSize = 0;

	uint* Indices = nullptr;
	uint IndicesID = 0; // index in VRAM
	uint IndicesSize = 0;
};

#endif

