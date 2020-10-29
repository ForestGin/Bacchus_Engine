#ifndef __RESOURCE_RENDERER_H__
#define __RESOURCE_RENDERER_H__

#include "Res.h"

#define NORMAL_LENGTH 1

class ResourceMesh;


class ResourceRenderer : public Res
{
public:

	ResourceRenderer(GameObject* ContainerGO);
	virtual ~ResourceRenderer();

	void Draw();

private:
	void DrawMesh(ResourceMesh& mesh) const;
	void DrawNormals(const ResourceMesh& mesh) const;
	void DrawAxis() const;

public:
	bool draw_vertexnormals = false;
	bool draw_facenormals = false;
	bool checkers = false;
};

#endif 
