#include "ModuleSceneManager.h"
#include "OpenGL.h"
#include "Application.h"
#include "GameObject.h"
#include "ModuleRenderer3D.h"
#include "ResourceRenderer.h"
#include "ResourceMaterial.h"
#include "ImporterMaterial.h"
#include "ResourceMesh.h"
#include "ModuleTextures.h"

#include "mmgr/mmgr.h"

ModuleSceneManager::ModuleSceneManager(bool start_enabled)
{
}

ModuleSceneManager::~ModuleSceneManager()
{
}

bool ModuleSceneManager::Init(json file)
{
	return true;
}

bool ModuleSceneManager::Start()
{
	CheckersMaterial = CreateEmptyMaterial();

	CheckersMaterial->TextureID = App->tex->GetCheckerTextureID();

	GameObject* cube = CreateCube(1, 1, 1, true);
	cube->SetPosition(3.0f, 1.0f, 0.0f);

	return true;
}

update_status ModuleSceneManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{

	for (uint i = 0; i < game_objects.size(); ++i)
	{
		if (game_objects[i])
			delete game_objects[i];
	}

	for (uint i = 0; i < Materials.size(); ++i)
	{
		if (Materials[i])
			delete Materials[i];
	}

	CheckersMaterial = nullptr;

	return true;
}

uint ModuleSceneManager::GetNumGameObjects() const
{
	return game_objects.size();
}

uint ModuleSceneManager::GetSelectedGameObjects()
{
	return SelectedGameObject;
}

std::vector<GameObject*>& ModuleSceneManager::GetGameObjects()
{
	return game_objects;
}

void ModuleSceneManager::SetSelectedGameObject(uint index)
{
	SelectedGameObject = index;
}

GameObject* ModuleSceneManager::CreateEmptyGameObject()
{
	std::string Name = "GameObject ";
	Name.append("(");
	Name.append(std::to_string(game_objects.size()));
	Name.append(")");

	GameObject* new_object = new GameObject(Name.data());
	game_objects.push_back(new_object);

	return new_object;
}

ResourceMaterial* ModuleSceneManager::CreateEmptyMaterial()
{

	ResourceMaterial* Material = new ResourceMaterial(Res::ResType::Material);

	
	Materials.push_back(Material);

	return Material;
}

void ModuleSceneManager::Draw() const
{
	CreateGrid();

	for (uint i = 0; i < game_objects.size(); ++i)
	{
		glPushMatrix();
		glMultMatrixf(game_objects[i]->GetLocalTransform().ptr());

		ResourceRenderer* Renderer = (ResourceRenderer*)game_objects[i]->GetResource(Res::ResType::Renderer);

		if (Renderer)
		{
			Renderer->Draw();
		}

		glPopMatrix();
	}

}

GameObject* ModuleSceneManager::CreateCube(float sizeX, float sizeY, float sizeZ, bool checkers)
{
	float3 size = float3(sizeX, sizeY, sizeZ);
	float sx = size.x * 0.5f;
	float sy = size.y * 0.5f;
	float sz = size.z * 0.5f;

	GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

	ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

	ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

	uint verticesSize = 3 * 8;
	new_mesh->VerticesSize = verticesSize;

	new_mesh->Vertices = new float3[verticesSize]{  // 8 of vertex coords

		{	sx, sy, sz },  {-sx, sy, sz },  {-sx,-sy, sz },  { sx,-sy, sz },  // v0,v1,v2,v3 (front)
		{	sx, sy, sz },  { sx,-sy, sz },  { sx,-sy,-sz },  { sx, sy,-sz },  // v0,v3,v4,v5 (right)
		{	sx, sy, sz },  { sx, sy,-sz },  {-sx, sy,-sz },  {-sx, sy, sz },  // v0,v5,v6,v1 (top)
		{  -sx, sy, sz },  {-sx, sy,-sz },  {-sx,-sy,-sz },  {-sx,-sy, sz },  // v1,v6,v7,v2 (left)
		{  -sx,-sy,-sz },  { sx,-sy,-sz },  { sx,-sy, sz },  {-sx,-sy, sz },  // v7,v4,v3,v2 (bottom)
		{	sx,-sy,-sz },  {-sx,-sy,-sz },  {-sx, sy,-sz },  { sx, sy,-sz }   // v4,v7,v6,v5 (back)
	};

	glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * verticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

	
	new_mesh->IndicesSize = 3 * (2 * 6);
	new_mesh->Indices = new uint[new_mesh->IndicesSize]{ 0, 1, 2,   2, 3, 0,    // v0-v1-v2, v2-v3-v0 (front)
									 4, 5, 6,   6, 7, 4,    // v0-v3-v4, v4-v5-v0 (right)
									 8, 9,10,  10,11, 8,    // v0-v5-v6, v6-v1-v0 (top)
									12,13,14,  14,15,12,    // v1-v6-v7, v7-v2-v1 (left)
									16,17,18,  18,19,16,    // v7-v4-v3, v3-v2-v7 (bottom)
									20,21,22,  22,23,20     // v4-v7-v6, v6-v5-v4 (back)
	};


	glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->IndicesSize, new_mesh->Indices, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

	

	new_mesh->TexCoordsSize = verticesSize * 2;

	new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{
		1, 0, 0, 0, 0, 1, 1, 1,               // v0,v1,v2,v3 (front)
			0, 0, 0, 1, 1, 1, 1, 0,               // v0,v3,v4,v5 (right)
			1, 1, 1, 0, 0, 0, 0, 1,               // v0,v5,v6,v1 (top)
			1, 0, 0, 0, 0, 1, 1, 1,               // v1,v6,v7,v2 (left)
			0, 1, 1, 1, 1, 0, 0, 0,               // v7,v4,v3,v2 (bottom)
			0, 1, 1, 1, 1, 0, 0, 0                // v4,v7,v6,v5 (back)
	};

	if (checkers)
		new_object->SetMaterial(CheckersMaterial);

	glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

	return new_object;
}

GameObject* CreateSphere(float Radius, int slices, int slacks, bool checkers = false)
{
	return nullptr;
}

void ModuleSceneManager::CreateGrid() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float distance = 200.0f;

	for (int max_linesgrid = -distance; max_linesgrid < distance; max_linesgrid++)
	{
		glVertex3f((float)max_linesgrid, 0.0f, -distance);
		glVertex3f((float)max_linesgrid, 0.0f, distance);
		glVertex3f(-distance, 0.0f, (float)max_linesgrid);
		glVertex3f(distance, 0.0f, (float)max_linesgrid);

	}

	glVertex3f((float)-distance, 0.0f, distance);
	glVertex3f((float)distance, 0.0f, distance);
	glVertex3f((float)distance, 0.0f, -distance);
	glVertex3f((float)distance, 0.0f, distance);

	glLineWidth(1.0f);

	glEnd();
}
