#ifndef __ModuleSceneIntro_H__
#define __ModuleSceneIntro_H__

#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <array>

using namespace std;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	uint cube_id;//array 

	//Cube
	uint cube_verticesID;
	uint cube_buffIndicesID;

	//Tetrahedron
	uint tetrahedron_verticesID;
	uint tetrahedron_buffIndicesID;

	//Octahedron
	uint octahedron_verticesID;
	uint octahedron_buffIndicesID;

	//Icosahedron
	uint icosahedron_verticesID;
	uint icosahedron_buffIndicesID;

	//Dodecahedron
	uint dodecahedron_verticesID;
	uint dodecahedron_buffIndicesID;

};

#endif
