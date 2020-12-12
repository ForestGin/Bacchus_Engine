#include "ModuleSceneManager.h"
#include "OpenGL.h"
#include "Application.h"
#include "GameObject.h"

#include "ModuleRenderer3D.h"
#include "ModuleTextures.h"
#include "ModuleImporter.h"
#include "ImporterMaterial.h"
#include "ImporterScene.h"
#include "FileSystem.h"
#include "ModuleResources.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentRenderer.h"
#include "ComponentMesh.h"

#include "ResourceMaterial.h"
#include "ResourceTexture.h"

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
    root = CreateRootGameObject();
    tree.SetBoundaries(AABB(float3(-10, 0, -10), float3(10, 10, 10)));

	return true;
}

bool ModuleSceneManager::Start()
{
    //Define Default and Checkers Materials
	DefaultMaterial = CreateEmptyMaterial();
    DefaultMaterial->resource_material = (ResourceMaterial*)App->res->CreateResource(Resource::ResourceType::MATERIAL);
    DefaultMaterial->resource_material->resource_diffuse = (ResourceTexture*)App->res->CreateResource(Resource::ResourceType::TEXTURE);

    DefaultMaterial->resource_material->resource_diffuse->Texture_path = "Default";

	CheckersMaterial = CreateEmptyMaterial();
    CheckersMaterial->resource_material = (ResourceMaterial*)App->res->CreateResource(Resource::ResourceType::MATERIAL);
    CheckersMaterial->resource_material->resource_diffuse = (ResourceTexture*)App->res->CreateResource(Resource::ResourceType::TEXTURE);

    CheckersMaterial->resource_material->resource_diffuse->buffer_id = App->tex->GetCheckerTextureID();
    CheckersMaterial->resource_material->resource_diffuse->Texture_path = "NaN";
    CheckersMaterial->resource_material->resource_diffuse->Texture_width = CHECKERS_WIDTH;
    CheckersMaterial->resource_material->resource_diffuse->Texture_height = CHECKERS_HEIGHT;

	return true;
}

update_status ModuleSceneManager::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneManager::Update(float dt)
{
    root->Update(dt);

	return UPDATE_CONTINUE;
}

bool ModuleSceneManager::CleanUp()
{
    root->RecursiveDelete();

	for (uint i = 0; i < Materials.size(); ++i)
	{
		if (Materials[i])
			delete Materials[i];
	}
	Materials.clear();
    NoStaticGo.clear();

    DefaultMaterial = nullptr;
	CheckersMaterial = nullptr;

	return true;
}

void ModuleSceneManager::Draw()
{
	CreateGrid();

    DrawScene();

}

void ModuleSceneManager::DrawScene()
{
    RecursiveDrawQuadtree(tree.root);

    for (std::vector<GameObject*>::iterator it = NoStaticGo.begin(); it != NoStaticGo.end(); it++)
    {
        if ((*it)->GetName() != root->GetName())
        {
            //Search for Renderer Component 
            ComponentRenderer* Renderer = (*it)->GetComponent<ComponentRenderer>(Component::ComponentType::Renderer);

            //If Found, draw the mesh
            if (Renderer && Renderer->IsEnabled())
                    Renderer->Draw();
        }
    }

    std::vector<GameObject*> static_go;
    tree.CollectIntersections(static_go, App->renderer3D->culling_camera->frustum);

    for (std::vector<GameObject*>::iterator it = static_go.begin(); it != static_go.end(); it++)
    {
        //Search for Renderer Component
        ComponentRenderer* Renderer = (*it)->GetComponent<ComponentRenderer>(Component::ComponentType::Renderer);

        if (Renderer && Renderer->IsEnabled())
            Renderer->Draw();
    }

    //
    if (App->camera->last_ray.IsFinite())
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);

        glColor4f(Red.r, Red.g, Red.b, Red.a);

        glVertex3fv((GLfloat*)&App->camera->last_ray.a);
        glVertex3fv((GLfloat*)&App->camera->last_ray.b);

        glColor4f(1.0, 1.0, 1.0, 1.0);

        glEnd();
        glEnable(GL_LIGHTING);
    }
}

GameObject* ModuleSceneManager::GetRootGO() const
{
    return root;
}

void ModuleSceneManager::RedoOctree()
{
    std::vector<GameObject*> scene_gos;
    tree.CollectObjects(scene_gos);

    tree.SetBoundaries(AABB(float3(-10, 0, -10), float3(10, 10, 10)));

    for (uint i = 0; i < scene_gos.size(); ++i)
    {
        //tree.Erase(scene_gos[i]);
        tree.Insert(scene_gos[i]);
    }

}

void ModuleSceneManager::SetStatic(GameObject* go)
{
    if (!go->Static)
    {
        tree.Insert(go);

        for (std::vector<GameObject*>::iterator it = NoStaticGo.begin(); it != NoStaticGo.end(); it++)
        {
            if ((*it) == go)
            {
                NoStaticGo.erase(it);
                break;
            }
        }
    }
    else 
    {
        NoStaticGo.push_back(go);
        tree.Erase(go);
    }
}

void ModuleSceneManager::RecursiveDrawQuadtree(QuadtreeNode* node) const
{
    if (!node->IsLeaf())
    {
        for (uint i = 0; i < 8; ++i)
        {
            RecursiveDrawQuadtree(node->childs[i]);
        }
    }

    DrawWire(node->box, Red);
}

void ModuleSceneManager::SelectFromRay(LineSegment& ray)
{
    //Static gos
    std::map<float, GameObject*> candidate_gos;
    tree.CollectIntersections(candidate_gos, ray);

    //Non-static gos
    for (std::vector<GameObject*>::iterator it = NoStaticGo.begin(); it != NoStaticGo.end(); it++)
    {
        if (ray.Intersects((*it)->GetAABB()))
        {
            float hit_near, hit_far;
            if (ray.Intersects((*it)->GetOBB(), hit_near, hit_far))
                candidate_gos[hit_near] = *it;
        }
    }

    GameObject* toSelect = nullptr;
    for (std::map<float, GameObject*>::iterator it = candidate_gos.begin(); it != candidate_gos.end() && toSelect == nullptr; it++)
    {
        //Testing Triangles
        ComponentMesh* mesh = it->second->GetComponent<ComponentMesh>(Component::ComponentType::Mesh);

        if (mesh)
        {
            ResourceMesh* resource_mesh = mesh->resource_mesh;

            if (mesh->resource_mesh)
            {
                LineSegment local = ray;
                local.Transform(it->second->GetComponent<ComponentTransform>(Component::ComponentType::Transform)->GetGlobalTransform().Inverted());
            
                for (uint j = 0; j < mesh->resource_mesh->IndicesSize / 3; j++)
                {
                    float3 a = mesh->resource_mesh->Vertices[mesh->resource_mesh->Indices[j * 3]];
                    float3 b = mesh->resource_mesh->Vertices[mesh->resource_mesh->Indices[(j * 3) + 1]];
                    float3 c = mesh->resource_mesh->Vertices[mesh->resource_mesh->Indices[(j * 3) + 2]];
                    
                    // Create Triangle given three vertices
                    Triangle triangle(a, b, c);

                    //Test ray/triangle intersection
                    if (local.Intersects(triangle, nullptr, nullptr))
                    {
                        toSelect = it->second;
                        break;
                    }
                }
            }
        }
    }

    //Set Selected
    if (toSelect)
        SelectedGameObject = toSelect;
}

void ModuleSceneManager::SaveStatus(json& file) const
{
}

void ModuleSceneManager::LoadStatus(const json& file)
{
}

void ModuleSceneManager::SaveScene()
{
    
    std::vector<GameObject*> scene_gos;
    GatherGameObjects(scene_gos, root);

    if (scene_gos.size() > 0)
    {
        std::string Scene_name = "SampleScene";
        App->importer->GetImporterScene()->SaveSceneToFile(scene_gos, Scene_name, SCENE);
    }
}

void ModuleSceneManager::LoadScene()
{
    std::string Scene_name = SCENES_FOLDER;
    Scene_name.append("SampleScene.scene");

    RecursiveFreeScene(root);

    if(App->fs->Exists(Scene_name.data()))
    App->importer->GetImporterScene()->Load(Scene_name.data());
}

void ModuleSceneManager::RecursiveFreeScene(GameObject* go)
{
    //Delete all objects except root (if go is root)

    if (go->childs.size() > 0)
    {
        for (std::vector<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); ++it)
        {
            RecursiveFreeScene(*it);
        }

        go->childs.clear();
    }

    if (go->GetName() != root->GetName())
    {        
        delete go;
    }
}

GameObject* ModuleSceneManager::GetSelectedGameObjects() const
{
    return SelectedGameObject;
}

void ModuleSceneManager::GatherGameObjects(std::vector<GameObject*>& scene_gos, GameObject* go)
{
    
    if (go->GetName() != root->GetName())
        scene_gos.push_back(go);

    if (go->childs.size() > 0)
    {
        for (std::vector<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); ++it)
        {
            GatherGameObjects(scene_gos, *it);
        }
    }
}

void ModuleSceneManager::SetSelectedGameObject(GameObject* go)
{
    SelectedGameObject = go;
}

void ModuleSceneManager::SetTextureToSelectedGO(uint id)
{
    ComponentMaterial* Material = SelectedGameObject->GetComponent<ComponentMaterial>(Component::ComponentType::Material);

    if (Material)
    {
        //Material->FreeTexture();
        Material->resource_material->resource_diffuse->buffer_id = id;
    }
}

GameObject* ModuleSceneManager::CreateEmptyGameObject()
{
    std::string Name = "GameObject ";
    Name.append("(");
    Name.append(std::to_string(go_count));
    Name.append(")");

    go_count++;

    GameObject* new_object = new GameObject(Name.data());
    NoStaticGo.push_back(new_object);

    new_object->AddComponent(Component::ComponentType::Transform);
    
    new_object->UpdateAABB();
    
    root->AddChildGO(new_object);

    new_object->SetMaterial(DefaultMaterial);

    return new_object;
}

GameObject* ModuleSceneManager::CreateRootGameObject()
{
    
    std::string Name = "root";
    GameObject* new_object = new GameObject(Name.data());
    new_object->AddComponent(Component::ComponentType::Transform);

    return new_object;
}

ComponentMaterial* ModuleSceneManager::CreateEmptyMaterial()
{
    ComponentMaterial* Material = new ComponentMaterial(Component::ComponentType::Material);

    Materials.push_back(Material);

    return Material;
}

void ModuleSceneManager::DrawWireFromVertices(const float3* corners, Color color)
{
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);

    glColor4f(color.r, color.g, color.b, color.a);

    //Between-planes right
    glVertex3fv((GLfloat*)&corners[1]);
    glVertex3fv((GLfloat*)&corners[5]);
    glVertex3fv((GLfloat*)&corners[7]);
    glVertex3fv((GLfloat*)&corners[3]);

    //Between-planes left
    glVertex3fv((GLfloat*)&corners[4]);
    glVertex3fv((GLfloat*)&corners[0]);
    glVertex3fv((GLfloat*)&corners[2]);
    glVertex3fv((GLfloat*)&corners[6]);

    //Far plane horizontal
    glVertex3fv((GLfloat*)&corners[5]);
    glVertex3fv((GLfloat*)&corners[4]);
    glVertex3fv((GLfloat*)&corners[6]);
    glVertex3fv((GLfloat*)&corners[7]);

    //Near plane horizontal
    glVertex3fv((GLfloat*)&corners[0]);
    glVertex3fv((GLfloat*)&corners[1]);
    glVertex3fv((GLfloat*)&corners[3]);
    glVertex3fv((GLfloat*)&corners[2]);

    //Near plane vertical
    glVertex3fv((GLfloat*)&corners[1]);
    glVertex3fv((GLfloat*)&corners[3]);
    glVertex3fv((GLfloat*)&corners[0]);
    glVertex3fv((GLfloat*)&corners[2]);

    //Far plane vertical
    glVertex3fv((GLfloat*)&corners[5]);
    glVertex3fv((GLfloat*)&corners[7]);
    glVertex3fv((GLfloat*)&corners[4]);
    glVertex3fv((GLfloat*)&corners[6]);

    glColor4f(1.0, 1.0, 1.0, 1.0);

    glEnd();
    glEnable(GL_LIGHTING);
}

void ModuleSceneManager::LoadPrimitiveArrays(GameObject& new_object, uint vertices_size, const float* vertices, uint indices_size, const uint* indices, uint normals_size, const float* normals, uint texCoords_size, const float* texCoords) const
{
    //Obtain data from Songho Ann mesh arrays and load it into component mesh

    ComponentMesh* comp_mesh = (ComponentMesh*)new_object.AddComponent(Component::ComponentType::Mesh);
    ResourceMesh* new_mesh = (ResourceMesh*)App->res->CreateResource(Resource::ResourceType::MESH);

    comp_mesh->resource_mesh = new_mesh;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object.AddComponent(Component::ComponentType::Renderer);

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

    new_mesh->CreateAABB();
}

GameObject* ModuleSceneManager::CreateSphere(float radius, int sectors, int stacks, bool smooth)
{
    GameObject* new_object = CreateEmptyGameObject();

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
    GameObject* new_object = CreateEmptyGameObject();

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
    GameObject* new_object = CreateEmptyGameObject();

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
    GameObject* new_object = CreateEmptyGameObject();

    vCylinder cylinder(baseRadius, topRadius, height, sectors, stacks, smooth);

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

void ModuleSceneManager::DestroyGameObject(GameObject* go)
{
    go->parent->RemoveChildGO(go);
    go->RecursiveDelete();

    this->go_count--;
}