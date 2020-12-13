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
#include "BacchusEditor.h"

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
    tree.SetBoundaries(AABB(float3(-100, -100, -100), float3(100, 100, 100)));

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

    //Create primitives
    tetrahedron = CreateCylinder(1, 0, 1, 3, 1, false);
    cube = CreateCubeSphere(1, 0, false);
    octahedron = CreateSphere(1, 4, 2, false);
    icosahedron = CreateIcoSphere(1, 0, false);
    sphere = CreateSphere(1, 36, 18, false);
    cubesphere = CreateCubeSphere(1, 3, false);
    icosphere = CreateIcoSphere(1, 3, false);
    pyramid = CreateCylinder(1, 0, 1, 4, 1, false);
    cylinder = CreateCylinder(1, 1, 1, 36, 4, false);
    cone = CreateCylinder(1, 0, 1, 36, 4, false);

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
    NoStaticGo.clear();

    for (uint i = 0; i < Materials.size(); ++i)
    {
        if (Materials[i])
            delete Materials[i];
    }
    Materials.clear();

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
    if (display_tree)
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

    //Draw RAy
    /*if (App->camera->last_ray.IsFinite())
    {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);

        glColor4f(Red.r, Red.g, Red.b, Red.a);

        glVertex3fv((GLfloat*)&App->camera->last_ray.a);
        glVertex3fv((GLfloat*)&App->camera->last_ray.b);

        glColor4f(1.0, 1.0, 1.0, 1.0);

        glEnd();
        glEnable(GL_LIGHTING);
    }*/
}

GameObject* ModuleSceneManager::GetRootGO() const
{
    return root;
}

void ModuleSceneManager::RedoOctree()
{
    std::vector<GameObject*> scene_gos;
    tree.CollectObjects(scene_gos);

    tree.SetBoundaries(AABB(float3(-100, -100, -100), float3(100, 100, 100)));

    for (uint i = 0; i < scene_gos.size(); ++i)
    {
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
    if (!App->bacchuseditor->IsMouseCaptured())
    {
        //Gather static gos 
        std::map<float, GameObject*> candidate_gos;
        tree.CollectIntersections(candidate_gos, ray);

        //Gather non-static gos
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
            //We have to test triangle by triangle
            ComponentMesh* mesh = it->second->GetComponent<ComponentMesh>(Component::ComponentType::Mesh);

            if (mesh)
            {

                if (mesh->resource_mesh)
                {
                    //We need to transform the ray to local mesh space
                    LineSegment local = ray;
                    local.Transform(it->second->GetComponent<ComponentTransform>(Component::ComponentType::Transform)->GetGlobalTransform().Inverted());

                    for (uint j = 0; j < mesh->resource_mesh->IndicesSize / 3; j++)
                    {
                        float3 a = mesh->resource_mesh->Vertices[mesh->resource_mesh->Indices[j * 3]];
                        float3 b = mesh->resource_mesh->Vertices[mesh->resource_mesh->Indices[(j * 3) + 1]];
                        float3 c = mesh->resource_mesh->Vertices[mesh->resource_mesh->Indices[(j * 3) + 2]];
                        
                        //Create Triangle given three vertices
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

    if (DefaultMaterial)
        DefaultMaterial->resource_material->resource_diffuse->instances++;

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

void ModuleSceneManager::LoadPrimitiveArrays(ResourceMesh* new_mesh, uint vertices_size, const float* vertices, uint indices_size, const uint* indices, uint normals_size, const float* normals, uint texCoords_size, const float* texCoords) const
{
    //Obtain data from Songho Ann mesh arrays and load it into component mesh

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

ResourceMesh* ModuleSceneManager::CreateSphere(float radius, int sectors, int stacks, bool smooth)
{
    ResourceMesh* new_mesh = (ResourceMesh*)App->res->CreateResource(Resource::ResourceType::MESH);

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
    
    LoadPrimitiveArrays(new_mesh, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_mesh;
}

ResourceMesh* ModuleSceneManager::CreateCubeSphere(float radius, int sub, bool smooth)
{
    ResourceMesh* new_mesh = (ResourceMesh*)App->res->CreateResource(Resource::ResourceType::MESH);

    vCubesphere cubesphere(radius, sub, smooth);

    uint vertices_size = cubesphere.getVertexCount();
    const float* vertices = cubesphere.getVertices();

    uint indices_size = cubesphere.getVertexCount();
    const uint* indices = cubesphere.getIndices();

    uint normals_size = cubesphere.getNormalCount();
    const float* normals = cubesphere.getNormals();

    uint texCoords_size = cubesphere.getTexCoordCount();
    const float* texCoords = cubesphere.getTexCoords();

    LoadPrimitiveArrays(new_mesh, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_mesh;
}

ResourceMesh* ModuleSceneManager::CreateIcoSphere(float radius, int subdivision, bool smooth)
{
    ResourceMesh* new_mesh = (ResourceMesh*)App->res->CreateResource(Resource::ResourceType::MESH);

    vIcosphere icosphere(radius,subdivision,smooth);

    uint vertices_size = icosphere.getVertexCount();
    const float* vertices = icosphere.getVertices();

    uint indices_size = icosphere.getVertexCount();
    const uint* indices = icosphere.getIndices();

    uint normals_size = icosphere.getNormalCount();
    const float* normals = icosphere.getNormals();

    uint texCoords_size = icosphere.getTexCoordCount();
    const float* texCoords = icosphere.getTexCoords();

    LoadPrimitiveArrays(new_mesh, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_mesh;
}

ResourceMesh* ModuleSceneManager::CreateCylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth)
{
    ResourceMesh* new_mesh = (ResourceMesh*)App->res->CreateResource(Resource::ResourceType::MESH);

    vCylinder cylinder(baseRadius, topRadius, height, sectors, stacks, smooth);

    uint vertices_size = cylinder.getVertexCount();
    const float* vertices = cylinder.getVertices();

    uint indices_size = cylinder.getVertexCount();
    const uint* indices = cylinder.getIndices();

    uint normals_size = cylinder.getNormalCount();
    const float* normals = cylinder.getNormals();

    uint texCoords_size = cylinder.getTexCoordCount();
    const float* texCoords = cylinder.getTexCoords();

    LoadPrimitiveArrays(new_mesh, vertices_size, vertices, indices_size, indices, normals_size, normals, texCoords_size, texCoords);

    return new_mesh;
}

GameObject* ModuleSceneManager::LoadTetrahedron()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = tetrahedron;
    tetrahedron->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadCube()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = cube;
    cube->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadOctahedron()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = octahedron;
    octahedron->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadIcosahedron()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = icosahedron;
    icosahedron->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadSphere()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = sphere;
    sphere->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadCubeSphere()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = cubesphere;
    cubesphere->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadIcoSphere()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = icosphere;
    icosphere->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadPyramid()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = pyramid;
    pyramid->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadCylinder()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = cylinder;
    cylinder->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

    return new_object;
}

GameObject* ModuleSceneManager::LoadCone()
{
    GameObject* new_object = CreateEmptyGameObject();
    ComponentMesh* comp_mesh = (ComponentMesh*)new_object->AddComponent(Component::ComponentType::Mesh);
    comp_mesh->resource_mesh = cone;
    cone->instances++;
    ComponentRenderer* Renderer = (ComponentRenderer*)new_object->AddComponent(Component::ComponentType::Renderer);

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