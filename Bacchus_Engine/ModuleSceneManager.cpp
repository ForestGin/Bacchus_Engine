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
#include "ResourceTransform.h"

#include "vSphere.h"
#include "vCubesphere.h"
#include "vIcosphere.h"
#include "vCylinder.h"

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
	DefaultMaterial = CreateEmptyMaterial();
	DefaultMaterial->Texture_path = "Default";

	CheckersMaterial = CreateEmptyMaterial();
	CheckersMaterial->TextureID = App->tex->GetCheckerTextureID();
	CheckersMaterial->Texture_path = "NONE";
	CheckersMaterial->Texture_width = CHECKERS_WIDTH;
	CheckersMaterial->Texture_height = CHECKERS_HEIGHT;

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
	game_objects.clear();

	for (uint i = 0; i < Materials.size(); ++i)
	{
		if (Materials[i])
			delete Materials[i];
	}
	Materials.clear();

	CheckersMaterial = nullptr;

	return true;
}

void ModuleSceneManager::Draw() const
{
	CreateGrid();

	for (uint i = 0; i < game_objects.size(); ++i)
	{

        ResourceTransform* transform = game_objects[i]->GetResource<ResourceTransform>(Res::ResType::Transform);

		glPushMatrix();
        glMultMatrixf(transform->GetLocalTransform().ptr());

		ResourceRenderer* Renderer = game_objects[i]->GetResource<ResourceRenderer>(Res::ResType::Renderer);

		if (Renderer)
		{
			Renderer->Draw();
		}

		glPopMatrix();
	}
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

void ModuleSceneManager::SetTextureToSelectedGO(uint id)
{
    ResourceMaterial* Material = game_objects[SelectedGameObject]->GetResource<ResourceMaterial>(Res::ResType::Material);

    if (Material)
    {
        Material->FreeTexture();
        Material->TextureID = id;
    }
}

GameObject* ModuleSceneManager::CreateEmptyGameObject()
{
    std::string Name = "GameObject ";
    Name.append("(");
    Name.append(std::to_string(game_objects.size()));
    Name.append(")");

    GameObject* new_object = new GameObject(Name.data());
    game_objects.push_back(new_object);
    new_object->AddResource(Res::ResType::Transform);

    new_object->SetMaterial(DefaultMaterial);

    return new_object;
}

ResourceMaterial* ModuleSceneManager::CreateEmptyMaterial()
{

    ResourceMaterial* Material = new ResourceMaterial(Res::ResType::Material);


    Materials.push_back(Material);

    return Material;
}

void ModuleSceneManager::LoadPrimitiveArrays(GameObject& new_object, uint vertices_size, const float* vertices, uint indices_size, const uint* indices, uint normals_size, const float* normals, uint texCoords_size, const float* texCoords) const
{
    ResourceMesh* new_mesh = (ResourceMesh*)new_object.AddResource(Res::ResType::Mesh);

    ResourceRenderer* Renderer = (ResourceRenderer*)new_object.AddResource(Res::ResType::Renderer);

    //Vertices
    new_mesh->VerticesSize = vertices_size / 3;
    new_mesh->Vertices = new float3[new_mesh->VerticesSize]{};

    for (uint i = 0; i < new_mesh->VerticesSize; ++i)
    {
        new_mesh->Vertices[i].x = vertices[3 * i];
        new_mesh->Vertices[i].y = vertices[(3 * i) + 1];
        new_mesh->Vertices[i].z = vertices[(3 * i) + 2];
    }

    new_mesh->VerticesID = App->renderer3D->CreateBufferFromData(GL_ARRAY_BUFFER, sizeof(float3) * new_mesh->VerticesSize, new_mesh->Vertices);

    //Indices
    new_mesh->IndicesSize = indices_size;
    new_mesh->Indices = new uint[new_mesh->IndicesSize]{};

    for (uint i = 0; i < new_mesh->IndicesSize; ++i)
    {
        new_mesh->Indices[i] = indices[i];
    }

    new_mesh->IndicesID = App->renderer3D->CreateBufferFromData(GL_ARRAY_BUFFER, sizeof(uint) * new_mesh->IndicesSize, new_mesh->Indices);

    //Normals
    new_mesh->NormalsSize = normals_size / 3;
    new_mesh->Normals = new float3[new_mesh->NormalsSize];

    for (uint i = 0; i < new_mesh->NormalsSize; ++i)
    {
        new_mesh->Normals[i].x = normals[3 * i];
        new_mesh->Normals[i].y = normals[(3 * i) + 1];
        new_mesh->Normals[i].z = normals[(3 * i) + 2];
    }

    //TexCoords
    new_mesh->TexCoordsSize = texCoords_size;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};

    for (uint i = 0; i < new_mesh->TexCoordsSize; ++i)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

    new_mesh->TextureCoordsID = App->renderer3D->CreateBufferFromData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords);
}

GameObject* ModuleSceneManager::CreateSphere(float radius, int sectors, int stacks, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    vSphere sphere(radius, sectors, stacks, smooth);

    sphere.setRadius(radius);
    sphere.setSectorCount(sectors);
    sphere.setStackCount(stacks);
    sphere.setSmooth(smooth);

    uint vertices_size = sphere.getVertexCount();
    const float* vertices = sphere.getVertices();

    uint indices_size = sphere.getVertexCount();
    const uint* indices = sphere.getIndices();

    uint normals_size = sphere.getNormalCount();
    const float* normals = sphere.getNormals();

    uint texCoords_size = sphere.getTexCoordCount();
    const float* texCoords = sphere.getTexCoords();

    LoadPrimitiveArrays(*new_object, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_object;
}

GameObject* ModuleSceneManager::CreateCubeSphere(float radius, int sub, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    vCubesphere cubesphere(radius, sub, smooth);

    /*cubesphere.setRadius(radius);
    cubesphere.setSubdivision(sub);
    cubesphere.setSmooth(smooth);*/

    uint vertices_size = cubesphere.getVertexCount();
    const float* vertices = cubesphere.getVertices();

    uint indices_size = cubesphere.getVertexCount();
    const uint* indices = cubesphere.getIndices();

    uint normals_size = cubesphere.getNormalCount();
    const float* normals = cubesphere.getNormals();

    uint texCoords_size = cubesphere.getTexCoordCount();
    const float* texCoords = cubesphere.getTexCoords();

    LoadPrimitiveArrays(*new_object, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_object;
}

GameObject* ModuleSceneManager::CreateIcoSphere(float radius, int subdivision, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    vIcosphere icosphere(radius,subdivision,smooth);

    /*icosphere.setRadius(radius);
    icosphere.setSubdivision(subdivision);
    icosphere.setSmooth(smooth);*/

    uint vertices_size = icosphere.getVertexCount();
    const float* vertices = icosphere.getVertices();

    uint indices_size = icosphere.getVertexCount();
    const uint* indices = icosphere.getIndices();

    uint normals_size = icosphere.getNormalCount();
    const float* normals = icosphere.getNormals();

    uint texCoords_size = icosphere.getTexCoordCount();
    const float* texCoords = icosphere.getTexCoords();

    LoadPrimitiveArrays(*new_object, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_object;
}

GameObject* ModuleSceneManager::CreateCylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    vCylinder cylinder(baseRadius, topRadius, height, sectors, stacks, smooth);

    //cylinder.set(baseRadius, topRadius, height, sectors, stacks, smooth);

    /*cylinder.setBaseRadius(baseRadius);
    cylinder.setTopRadius(topRadius);
    cylinder.setHeight(height);
    cylinder.setSectorCount(sectors);
    cylinder.setStackCount(stacks);
    cylinder.setSmooth(smooth);*/

    uint vertices_size = cylinder.getVertexCount();
    const float* vertices = cylinder.getVertices();

    uint indices_size = cylinder.getVertexCount();
    const uint* indices = cylinder.getIndices();

    uint normals_size = cylinder.getNormalCount();
    const float* normals = cylinder.getNormals();

    uint texCoords_size = cylinder.getTexCoordCount();
    const float* texCoords = cylinder.getTexCoords();

    LoadPrimitiveArrays(*new_object, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_object;
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
