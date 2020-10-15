#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"


ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{
}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));


	float cubeArray[] = { 
		-1.f, 0.f, 0.f, 
		0.f, 0.f, 0.f, 
		-1.f, 1.f, 0.f,
		//
		0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 
		-1.f, 1.f, 0.f,
		//
		0.f, 0.f, 0.f, 
		0.f, 0.f, -1.f, 
		0.f, 1.f, 0.f,
		//
		0.f, 0.f, -1.f,
		0.f, 1.f, -1.f,
		0.f, 1.f, 0.f,
		//
		0.f, 0.f, -1.f, 
		0.f, 1.f, -1.f, 
		-1.f, 0.f, -1.f, 
		//
		0.f, 1.f, -1.f, 
		-1.f, 1.f, -1.f, 
		-1.f, 0.f, -1.f,
		//
		-1.f, 1.f, -1.f, 
		-1.f, 1.f, 0.f,
		-1.f, 0.f, -1.f, 
		//
		-1.f, 0.f, -1.f, 
		-1.f, 1.f, 0.f,
		-1.f, 0.f, 0.f,
		//
	    -1.f, 0.f, 0.f, 
		 0.f, 0.f, 0.f,
		-1.f, 0.f, -1.f, 
		//
		 0.f, 0.f, 0.f,
		 0.f, 0.f, -1.f, 
		-1.f, 0.f, -1.f,
		//
		-1.f, 1.f, 0.f, 
		 0.f, 1.f, 0.f, 
		-1.f, 1.f, -1.f, 
		//
		0.f, 1.f, 0.f, 
		0.f, 1.f, -1.f,
		-1.f, 1.f, -1.f
	};

	cube_id = 0;
	glGenBuffers(1, (GLuint*) & (cube_id));
	glBindBuffer(GL_ARRAY_BUFFER, cube_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, cubeArray, GL_STATIC_DRAW);

	//-------------------CUBE VERTEX & INDICES ARRAYS-------------------

	cube_verticesID = 0;
	const uint cube_size = 8 * 3;
	float cube_vertices_arr[cube_size] = {
		2.f,1.f,.0f,   
		1.f,1.f,.0f,   
		1.f,.0f,.0f,   
		2.f,.0f,.0f,    
		2.f,.0f,-1.f,   
		2.f,1.f,-1.f,   
		1.f,1.f,-1.f,   
		1.f,.0f,-1.f 
	};

	glGenBuffers(1, (GLuint*) & (cube_verticesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_verticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * cube_size, cube_vertices_arr, GL_STATIC_DRAW);

	cube_buffIndicesID = 0;
	const uint cube_indices = 6 * 2 * 3;
	uint cube_indices_arr[cube_indices] = {
		0,1,2,  2,3,0, 
		0,3,4,  4,5,0,  
		0,5,6,  6,1,0,  
		1,6,7,  7,2,1, 
		7,4,3,  3,2,7, 
		4,7,6,  6,5,4
	};

	glGenBuffers(1, (GLuint*) & (cube_buffIndicesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_buffIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * cube_indices, cube_indices_arr, GL_STATIC_DRAW);

	//-------------------TETRAHEDRON VERTEX & INDICES ARRAYS-------------------

	tetrahedron_verticesID = 0;
	const uint tetrahedron_size = 4 * 3;
	float tetrahedron_vertices_arr[tetrahedron_size] = {
		0.000, 1.333, 0,
		0.943, 0, 0,
		-0.471, 0, 0.816,
		-0.471, 0, -0.816,
	};

	//homemade translate z (tope de cutre kill me pls)
	int i = 2;
	while(i <= tetrahedron_size +1)
	{
		tetrahedron_vertices_arr[i] += 2; //z
		i += 3;
	}

	glGenBuffers(1, (GLuint*)&(tetrahedron_verticesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetrahedron_verticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * tetrahedron_size, tetrahedron_vertices_arr, GL_STATIC_DRAW);

	tetrahedron_buffIndicesID = 0;
	const uint tetrahedron_indices = 4 * 3;
	uint tetrahedron_indices_arr[tetrahedron_indices] = {
		2,1,0,
		3,2,0,
		1,3,0,
		1,2,3,
	};

	glGenBuffers(1, (GLuint*)&(tetrahedron_buffIndicesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetrahedron_buffIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * tetrahedron_indices, tetrahedron_indices_arr, GL_STATIC_DRAW);

	//-------------------OCTAHEDRON VERTEX & INDICES ARRAYS-------------------

	octahedron_verticesID = 0;
	const uint octahedron_size = 6 * 3;
	float octahedron_vertices_arr[octahedron_size] = {
		0.000, 0.000, 1.000,
		1.000, 0.000, 0.000,
		0.000, 1.000, 0.000,
		-1.000, 0.000, 0.000,
		0.000, -1.000, 0.000,
		0.000, 0.000, -1.000
	};

	//homemade translate
	i = 0;
	while (i <= octahedron_size)
	{
		octahedron_vertices_arr[i] += -2; //x
		octahedron_vertices_arr[i+1] += 1; //y
		i += 3;
	}

	glGenBuffers(1, (GLuint*)&(octahedron_verticesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octahedron_verticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * octahedron_size, octahedron_vertices_arr, GL_STATIC_DRAW);

	octahedron_buffIndicesID = 0;
	const uint octahedron_indices = 8 * 3;
	uint octahedron_indices_arr[octahedron_indices] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,1,
		2,1,5,
		3,2,5,
		4,3,5,
		1,4,5,
	};

	glGenBuffers(1, (GLuint*)&(octahedron_buffIndicesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octahedron_buffIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * octahedron_indices, octahedron_indices_arr, GL_STATIC_DRAW);

	//-------------------ICOSAHEDRON VERTEX & INDICES ARRAYS-------------------

	icosahedron_verticesID = 0;
	const uint icosahedron_size = 12 * 3;
	float icosahedron_vertices_arr[icosahedron_size] = {
		0.000,  0.000,  1.000,
		0.894,  0.000,  0.447,
		0.276,  0.851,  0.447,
		-0.724,  0.526,  0.447,
		-0.724, -0.526,  0.447,
		0.276, -0.851,  0.447,
		0.724,  0.526, -0.447,
		-0.276,  0.851, -0.447,
		-0.894,  0.000, -0.447,
		-0.276, -0.851, -0.447,
		0.724, -0.526, -0.447,
		0.000,  0.000, -1.000
	};

	//homemade translate
	i = 0;
	while (i <= icosahedron_size)
	{
		icosahedron_vertices_arr[i] += -2; //x
		icosahedron_vertices_arr[i + 1] += 1; //y
		icosahedron_vertices_arr[i + 2] += 2; //z
		i += 3;
	}

	glGenBuffers(1, (GLuint*)&(icosahedron_verticesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedron_verticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * icosahedron_size, icosahedron_vertices_arr, GL_STATIC_DRAW);

	icosahedron_buffIndicesID = 0;
	const uint icosahedron_indices = 20 * 3;
	uint icosahedron_indices_arr[icosahedron_indices] = {
		0,1,2,
		0,2,3,
		0,3,4,
		0,4,5,
		0,5,1,
		7,6,11,
		8,7,11,
		9,8,11,
		10,9,11,
		6,10,11,
		6,2,1,
		7,3,2,
		8,4,3,
		9,5,4,
		10,1,5,
		6,7,2,
		7,8,3,
		8,9,4,
		9,10,5,
		10,6,1
	};

	glGenBuffers(1, (GLuint*)&(icosahedron_buffIndicesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedron_buffIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * icosahedron_indices, icosahedron_indices_arr, GL_STATIC_DRAW);

	//-------------------DODECAHEDRON VERTEX & INDICES ARRAYS-------------------

	dodecahedron_verticesID = 0;
	const uint dodecahedron_size = 20 * 3;
	float dodecahedron_vertices_arr[dodecahedron_size] = {
		0.607, 0.000, 0.795,
		0.188, 0.577, 0.795,
		-0.491, 0.357, 0.795,
		-0.491, -0.357, 0.795,
		0.188, -0.577, 0.795,
		0.982, 0.000, 0.188,
		0.304, 0.934, 0.188,
		-0.795, 0.577, 0.188,
		-0.795, -0.577, 0.188,
		0.304, -0.934, 0.188,
		0.795, 0.577, -0.188,
		-0.304, 0.934, -0.188,
		-0.982, 0.000, -0.188,
		-0.304, -0.934, -0.188,
		0.795, -0.577, -0.188,
		0.491, 0.357, -0.795,
		-0.188, 0.577, -0.795,
		-0.607, 0.000, -0.795,
		-0.188, -0.577, -0.795,
		0.491, -0.357, -0.795,
	};

	//homemade translate
	i = 0;
	while (i <= dodecahedron_size)
	{
		dodecahedron_vertices_arr[i] += -2; //x
		dodecahedron_vertices_arr[i + 1] += 1; //y
		dodecahedron_vertices_arr[i + 2] += 4; //z
		i += 3;
	}

	glGenBuffers(1, (GLuint*)&(dodecahedron_verticesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dodecahedron_verticesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * dodecahedron_size, dodecahedron_vertices_arr, GL_STATIC_DRAW);

	dodecahedron_buffIndicesID = 0;
	const uint dodecahedron_indices = 12 * 5;
	uint dodecahedron_indices_arr[dodecahedron_indices] = {
		0,1,2,3,4,
		5,10,6,1,0,
		6,11,7,2,1,
		7,12,8,3,2,
		8,13,9,4,3,
		9,14,5,0,4,
		15,16,11,6,10,
		16,17,12,7,11,
		17,18,13,8,12,
		18,19,14,9,13,
		19,15,10,5,14,
		19,18,17,16,15
	};

	glGenBuffers(1, (GLuint*)&(dodecahedron_buffIndicesID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dodecahedron_buffIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * dodecahedron_indices, dodecahedron_indices_arr, GL_STATIC_DRAW);

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");


	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	PrimiPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();


	return UPDATE_CONTINUE;
}

update_status ModuleSceneIntro::PostUpdate(float dt)
{

	//TEST DRAW CUBE OPENGL USING DIRECT MODE
	
	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	glColor3f(1.f, 1.f, 1.f);

	//glVertex3f(1.f, 1.f, 0.f); //v0
	//glVertex3f(1.f, 0.f, 0.f); //v1
	//glVertex3f(0.f, 0.f, 0.f); //v2
	//glVertex3f(0.f, 1.f, 0.f); //v3
	//glVertex3f(1.f, 0.f, -1.f);//v4
	//glVertex3f(1.f, 1.f, -1.f);//v5
	//glVertex3f(0.f, 1.f, -1.f);//v6
	//glVertex3f(0.f, 0.f, -1.f);//v7

	//front face
	glVertex3f(1.f, 1.f, 0.f); //v0
	glVertex3f(1.f, 0.f, 0.f); //v1
	glVertex3f(0.f, 0.f, 0.f); //v2
	
	glVertex3f(0.f, 0.f, 0.f); //v2
	glVertex3f(0.f, 1.f, 0.f); //v3
	glVertex3f(1.f, 1.f, 0.f); //v0

	//right face
	glVertex3f(1.f, 1.f, 0.f); //v0
	glVertex3f(0.f, 1.f, 0.f); //v3
	glVertex3f(1.f, 0.f, -1.f);//v4
	
	glVertex3f(1.f, 0.f, -1.f);//v4
	glVertex3f(1.f, 1.f, -1.f);//v5 
	glVertex3f(1.f, 1.f, 0.f); //v0

	//top face
	glVertex3f(1.f, 1.f, 0.f); //v0
	glVertex3f(1.f, 1.f, -1.f);//v5
	glVertex3f(0.f, 1.f, -1.f);//v6
	
	glVertex3f(0.f, 1.f, -1.f);//v6
	glVertex3f(1.f, 0.f, 0.f); //v1
	glVertex3f(1.f, 1.f, 0.f); //v0

	//left face
	glVertex3f(1.f, 0.f, 0.f); //v1
	glVertex3f(0.f, 1.f, -1.f);//v6
	glVertex3f(0.f, 0.f, -1.f);//v7
	
	glVertex3f(0.f, 0.f, -1.f);//v7
	glVertex3f(0.f, 0.f, 0.f); //v2
	glVertex3f(1.f, 0.f, 0.f); //v1

	//bottom face
	glVertex3f(0.f, 0.f, -1.f);//v7
	glVertex3f(1.f, 0.f, -1.f);//v4
	glVertex3f(0.f, 1.f, 0.f); //v3
	
	glVertex3f(0.f, 1.f, 0.f); //v3
	glVertex3f(0.f, 0.f, 0.f); //v2
	glVertex3f(0.f, 0.f, -1.f);//v7

	//back face
	glVertex3f(1.f, 0.f, -1.f);//v4
	glVertex3f(0.f, 0.f, -1.f);//v7
	glVertex3f(0.f, 1.f, -1.f);//v6

	glVertex3f(0.f, 1.f, -1.f);//v6
	glVertex3f(1.f, 1.f, -1.f);//v5
	glVertex3f(1.f, 0.f, -1.f);//v4
	
	glEnd();

	//(not direct mode)---------------------------------------------------------

	//------------------ENABLE------------------
	glEnableClientState(GL_VERTEX_ARRAY);
	
	//USING GL_VERTEX_ARRAYS

	//CUBE
	glBindBuffer(GL_ARRAY_BUFFER, cube_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(100.f, 51.f, 55.f);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//USING INDICES

	//CUBE
	glBindBuffer(GL_ARRAY_BUFFER, cube_verticesID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(87.f, 164.f, 229.f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_buffIndicesID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	//TETRAHEDRON
	glBindBuffer(GL_ARRAY_BUFFER, tetrahedron_verticesID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(66.f, 135.f, 245.f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tetrahedron_buffIndicesID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	//OCTAHEDRON
	glBindBuffer(GL_ARRAY_BUFFER, octahedron_verticesID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(2.f, 2.f, 2.f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, octahedron_buffIndicesID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);

	//ICOSAHEDRON
	glBindBuffer(GL_ARRAY_BUFFER, icosahedron_verticesID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(66.f, 135.f, 245.f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icosahedron_buffIndicesID);
	glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, NULL);
	
	//DODECAHEDRON
	glBindBuffer(GL_ARRAY_BUFFER, dodecahedron_verticesID);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(100.f, 51.f, 55.f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dodecahedron_buffIndicesID);
	glDrawElements(GL_POLYGON, 60, GL_UNSIGNED_INT, NULL);

	

	//------------------DISABLE------------------
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}