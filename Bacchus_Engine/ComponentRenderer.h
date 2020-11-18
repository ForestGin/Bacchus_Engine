#ifndef __COMPONENT_RENDERER_H__
#define __COMPONENT_RENDERER_H__

#include "Component.h"

#define NORMAL_LENGTH 1

class ComponentMesh;


class ComponentRenderer : public Component
{
public:

	ComponentRenderer(GameObject* ContainerGO);
	virtual ~ComponentRenderer();

	void Draw() const;

private:
	inline void DrawMesh(ComponentMesh& mesh) const;
	inline void DrawNormals(const ComponentMesh& mesh) const;
	inline void DrawAxis() const;

public:
	bool draw_vertexnormals = false;
	bool draw_facenormals = false;
	bool checkers = false;
};

#endif 