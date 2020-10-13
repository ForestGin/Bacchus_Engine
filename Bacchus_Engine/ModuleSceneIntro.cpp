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


	float cubeArray[] = { -1.f, 0.f, 0.f, 
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

	//TEST DRAW OPENGL USING DIRECT MODE
	glDisable(GL_CULL_FACE);

	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

	glColor3f(1.f, 1.f, 1.f);
	//face1
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	//
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, 0.f);
	//face2
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(0.f, 1.f, -1.f);
	//
	glVertex3f(1.f, 1.f, 0.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(0.f, 1.f, -1.f);
	//face3
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(1.f, 1.f, 0.f);
	//
	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(1.f, 1.f, 0.f);
	//face4
	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(1.f, 1.f, -1.f);
	glVertex3f(0.f, 1.f, -1.f);
	//
	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glVertex3f(0.f, 0.f, -1.f);
	//face5
	glVertex3f(0.f, 0.f, -1.f);
	glVertex3f(0.f, 1.f, -1.f);
	glVertex3f(0.f, 1.f, 0.f);
	//
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);
	//face6
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, -1.f);
	//
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, -1.f);
	glVertex3f(0.f, 0.f, -1.f);

	glEnd();

	//USING GL_VERTEX_ARRAYS

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, cube_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glColor3b(100, 51, 55);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableClientState(GL_VERTEX_ARRAY);

	return UPDATE_CONTINUE;
}