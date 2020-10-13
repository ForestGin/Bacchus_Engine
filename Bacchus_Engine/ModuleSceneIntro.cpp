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

	//TEST DRAW OPENGL USING DIRECT MODE

	glLineWidth(2.0f);

	glBegin(GL_TRIANGLES);

		glColor3f(1.f,1.f,1.f);
		//
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(2.f, 0.f, 0.f);
		glVertex3f(0.f, 2.f, 0.f);
		//
		glVertex3f(2.f, 0.f, 0.f);
		glVertex3f(2.f, 2.f, 0.f);
		glVertex3f(0.f, 2.f, 0.f);
		//
		glVertex3f(0.f, 2.f, 0.f);
		glVertex3f(2.f, 2.f, 0.f);
		glVertex3f(0.f, 2.f, -2.f);
		//
		glVertex3f(2.f, 2.f, 0.f);
		glVertex3f(2.f, 2.f, -2.f);
		glVertex3f(0.f, 2.f, -2.f);
		//
		glVertex3f(2.f, 0.f, 0.f);
		glVertex3f(2.f, 0.f, -2.f);
		glVertex3f(2.f, 2.f, 0.f);
		//
		glVertex3f(2.f, 0.f, -2.f);
		glVertex3f(2.f, 2.f, -2.f);
		glVertex3f(2.f, 2.f, 0.f);
		//
		glVertex3f(2.f, 0.f, -2.f);
		glVertex3f(2.f, 2.f, -2.f);
		glVertex3f(0.f, 2.f, -2.f);
		//
		glVertex3f(2.f, 0.f, -2.f);
		glVertex3f(0.f, 2.f, -2.f);
		glVertex3f(0.f, 0.f, -2.f);
		//
		glVertex3f(0.f, 0.f, -2.f);
		glVertex3f(0.f, 2.f, -2.f);
		glVertex3f(0.f, 2.f, 0.f);
		//
		glVertex3f(0.f, 2.f, 0.f);
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, -2.f);
		//
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(2.f, 0.f, 0.f);
		glVertex3f(0.f, 0.f, -2.f);
		//
		glVertex3f(2.f, 0.f, 0.f);
		glVertex3f(2.f, 0.f, -2.f);
		glVertex3f(0.f, 0.f, -2.f);

	glEnd();

	//USING GL_VERTEX_ARRAYS
	//uint my_id = 0;
	//GLfloat* vertices = new GLfloat[36 * 3];

	//glGenBuffers(1, (GLuint*) & (my_id));
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 3, vertices, GL_STATIC_DRAW);

	//glEnableClientState(GL_VERTEX_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, my_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//// … bind and use other buffers
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	//glDisableClientState(GL_VERTEX_ARRAY);
	

	return UPDATE_CONTINUE;
}