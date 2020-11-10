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
#include "Math.h"

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
    root->RecursiveDelete(root);

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
        glMultMatrixf(transform->GetLocalTransform().Transposed().ptr());

		ResourceRenderer* Renderer = game_objects[i]->GetResource<ResourceRenderer>(Res::ResType::Renderer);

		if (Renderer)
		{
			Renderer->Draw();
		}

		glPopMatrix();
	}
}

GameObject* ModuleSceneManager::GetRootGO() const
{
    return root;
}


uint ModuleSceneManager::GetNumGameObjects() const
{
    return game_objects.size();
}

GameObject* ModuleSceneManager::GetSelectedGameObjects()
{
    return SelectedGameObject;
}

std::vector<GameObject*>& ModuleSceneManager::GetGameObjects()
{
    return game_objects;
}

void ModuleSceneManager::SetSelectedGameObject(GameObject* go)
{
    SelectedGameObject = go;
}

void ModuleSceneManager::SetTextureToSelectedGO(uint id)
{
    ResourceMaterial* Material = SelectedGameObject->GetResource<ResourceMaterial>(Res::ResType::Material);

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
    root->AddChildGO(new_object);
    game_objects.push_back(new_object);
    new_object->AddResource(Res::ResType::Transform);

    new_object->SetMaterial(DefaultMaterial);

    return new_object;
}

GameObject* ModuleSceneManager::CreateRootGameObject()
{
    // --- Create New Game Object Name ---
    std::string Name = "root";

    // --- Create empty Game object to be filled out ---
    GameObject* new_object = new GameObject(Name.data());

    return new_object;
}

ResourceMaterial* ModuleSceneManager::CreateEmptyMaterial()
{

    ResourceMaterial* Material = new ResourceMaterial(Res::ResType::Material);


    Materials.push_back(Material);

    return Material;
}

GameObject* ModuleSceneManager::CreateSphere(float radius, int sectors, int stacks, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

    ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);
    
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    if (sectors < 3)
    {
        sectors = 3;
    }

    if (stacks < 2)
    {
        stacks = 2;
    }

	if (smooth)
	{
		//Sphere with smooth vertices
	}
    else
    {
        //Sphere with flat vertices
        const float PI = acos(-1);

        // tmp vertex definition (x,y,z,s,t)
        struct Vertex
        {
            float x, y, z, s, t;
        };
        std::vector<Vertex> tmpVertices;

        float sectorStep = 2 * PI / sectors;
        float stackStep = PI / stacks;
        float sectorAngle, stackAngle;

        // compute all vertices first, each vertex contains (x,y,z,s,t) except normal
        for (int i = 0; i <= stacks; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            float xy = radius * cosf(stackAngle);       // r * cos(u)
            float z = radius * sinf(stackAngle);        // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectors; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                Vertex vertex;
                vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
                vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
                vertex.z = z;                           // z = r * sin(u)
                vertex.s = (float)j / sectors;        // s
                vertex.t = (float)i / stacks;         // t
                tmpVertices.push_back(vertex);
            }
        }

        Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
        std::vector<float> n;                           // 1 face normal

        int i, j, k, vi1, vi2;
        int index = 0;                                  // index for vertex
        for (i = 0; i < stacks; ++i)
        {
            vi1 = i * (sectors + 1);                // index of tmpVertices
            vi2 = (i + 1) * (sectors + 1);

            for (j = 0; j < sectors; ++j, ++vi1, ++vi2)
            {
                // get 4 vertices per sector
                //  v1--v3
                //  |    |
                //  v2--v4
                v1 = tmpVertices[vi1];
                v2 = tmpVertices[vi2];
                v3 = tmpVertices[vi1 + 1];
                v4 = tmpVertices[vi2 + 1];

                // if 1st stack and last stack, store only 1 triangle per sector
                // otherwise, store 2 triangles (quad) per sector
                if (i == 0) // a triangle for first stack ==========================
                {
                    // put a triangle
                    vertices.push_back(v1.x );
                    vertices.push_back(v1.y);
                    vertices.push_back(v1.z);

                    vertices.push_back(v2.x);
                    vertices.push_back(v2.y);
                    vertices.push_back(v2.z);

                    vertices.push_back(v4.x);
                    vertices.push_back(v4.y);
                    vertices.push_back(v4.z);

                    // put tex coords of triangle
                    texCoords.push_back(v1.s);
                    texCoords.push_back(v1.t);

                    texCoords.push_back(v2.s);
                    texCoords.push_back(v2.t);

                    texCoords.push_back(v4.s);
                    texCoords.push_back(v4.t);

                    // put normal
                    const float EPSILON = 0.000001f;

                    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
                    float nx, ny, nz;

                    // find 2 edge vectors: v1-v2, v1-v3
                    float ex1 = v2.x - v1.x;
                    float ey1 = v2.y - v1.y;
                    float ez1 = v2.z - v1.z;
                    float ex2 = v4.x - v1.x;
                    float ey2 = v4.y - v1.y;
                    float ez2 = v4.z - v1.z;

                    // cross product: e1 x e2
                    nx = ey1 * ez2 - ez1 * ey2;
                    ny = ez1 * ex2 - ex1 * ez2;
                    nz = ex1 * ey2 - ey1 * ex2;

                    // normalize only if the length is > 0
                    float length = sqrtf(nx * nx + ny * ny + nz * nz);
                    if (length > EPSILON)
                    {
                        // normalize
                        float lengthInv = 1.0f / length;
                        normal[0] = nx * lengthInv;
                        normal[1] = ny * lengthInv;
                        normal[2] = nz * lengthInv;
                    }

                    n = normal; 
                    for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                    {
                        normals.push_back(n[0]);
                        normals.push_back(n[1]);
                        normals.push_back(n[2]);
                    }

                    // put indices of 1 triangle
                    indices.push_back(index);
                    indices.push_back(index + 1);
                    indices.push_back(index + 2);

                    index += 3;     // for next
                }
                else if (i == (stacks - 1)) // a triangle for last stack =========
                {
                    // put a triangle
                    vertices.push_back(v1.x);
                    vertices.push_back(v1.y);
                    vertices.push_back(v1.z);

                    vertices.push_back(v2.x);
                    vertices.push_back(v2.y);
                    vertices.push_back(v2.z);

                    vertices.push_back(v3.x);
                    vertices.push_back(v3.y);
                    vertices.push_back(v3.z);

                    // put tex coords of triangle
                    texCoords.push_back(v1.s);
                    texCoords.push_back(v1.t);

                    texCoords.push_back(v2.s);
                    texCoords.push_back(v2.t);

                    texCoords.push_back(v3.s);
                    texCoords.push_back(v3.t);

                    // put normal
                    const float EPSILON = 0.000001f;

                    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
                    float nx, ny, nz;

                    // find 2 edge vectors: v1-v2, v1-v3
                    float ex1 = v2.x - v1.x;
                    float ey1 = v2.y - v1.y;
                    float ez1 = v2.z - v1.z;
                    float ex2 = v3.x - v1.x;
                    float ey2 = v3.y - v1.y;
                    float ez2 = v3.z - v1.z;

                    // cross product: e1 x e2
                    nx = ey1 * ez2 - ez1 * ey2;
                    ny = ez1 * ex2 - ex1 * ez2;
                    nz = ex1 * ey2 - ey1 * ex2;

                    // normalize only if the length is > 0
                    float length = sqrtf(nx * nx + ny * ny + nz * nz);
                    if (length > EPSILON)
                    {
                        // normalize
                        float lengthInv = 1.0f / length;
                        normal[0] = nx * lengthInv;
                        normal[1] = ny * lengthInv;
                        normal[2] = nz * lengthInv;
                    }

                    n = normal;
                    for (k = 0; k < 3; ++k)  // same normals for 3 vertices
                    {
                        normals.push_back(n[0]);
                        normals.push_back(n[1]);
                        normals.push_back(n[2]);
                    }

                    // put indices of 1 triangle
                    indices.push_back(index);
                    indices.push_back(index + 1);
                    indices.push_back(index + 2);

                    index += 3;     // for next
                }
                else // 2 triangles for others ====================================
                {
                    // put quad vertices: v1-v2-v3-v4
                    vertices.push_back(v1.x);
                    vertices.push_back(v1.y);
                    vertices.push_back(v1.z);

                    vertices.push_back(v2.x);
                    vertices.push_back(v2.y);
                    vertices.push_back(v2.z);

                    vertices.push_back(v3.x);
                    vertices.push_back(v3.y);
                    vertices.push_back(v3.z);

                    vertices.push_back(v4.x);
                    vertices.push_back(v4.y);
                    vertices.push_back(v4.z);

                    // put tex coords of quad
                    texCoords.push_back(v1.s);
                    texCoords.push_back(v1.t);

                    texCoords.push_back(v2.s);
                    texCoords.push_back(v2.t);

                    texCoords.push_back(v3.s);
                    texCoords.push_back(v3.t);

                    texCoords.push_back(v4.s);
                    texCoords.push_back(v4.t);

                    // put normal
                    const float EPSILON = 0.000001f;

                    std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
                    float nx, ny, nz;

                    // find 2 edge vectors: v1-v2, v1-v3
                    float ex1 = v2.x - v1.x;
                    float ey1 = v2.y - v1.y;
                    float ez1 = v2.z - v1.z;
                    float ex2 = v3.x - v1.x;
                    float ey2 = v3.y - v1.y;
                    float ez2 = v3.z - v1.z;

                    // cross product: e1 x e2
                    nx = ey1 * ez2 - ez1 * ey2;
                    ny = ez1 * ex2 - ex1 * ez2;
                    nz = ex1 * ey2 - ey1 * ex2;

                    // normalize only if the length is > 0
                    float length = sqrtf(nx * nx + ny * ny + nz * nz);
                    if (length > EPSILON)
                    {
                        // normalize
                        float lengthInv = 1.0f / length;
                        normal[0] = nx * lengthInv;
                        normal[1] = ny * lengthInv;
                        normal[2] = nz * lengthInv;
                    }

                    n = normal;
                    for (k = 0; k < 4; ++k)  // same normals for 4 vertices
                    {
                        normals.push_back(n[0]);
                        normals.push_back(n[1]);
                        normals.push_back(n[2]);
                    }

                    // put indices of quad (2 triangles)
                    indices.push_back(index);
                    indices.push_back(index + 1);
                    indices.push_back(index + 2);

                    indices.push_back(index + 2);
                    indices.push_back(index + 1);
                    indices.push_back(index + 3);              
       
                    index += 4;     // for next
                }
            }
        }

        // generate interleaved vertex array as well
        /*std::vector<float>().swap(interleavedVertices);

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            interleavedVertices.push_back(vertices[i]);
            interleavedVertices.push_back(vertices[i + 1]);
            interleavedVertices.push_back(vertices[i + 2]);

            interleavedVertices.push_back(normals[i]);
            interleavedVertices.push_back(normals[i + 1]);
            interleavedVertices.push_back(normals[i + 2]);

            interleavedVertices.push_back(texCoords[j]);
            interleavedVertices.push_back(texCoords[j + 1]);
        }*/
    }

    //Vertices
    uint verticesSize = vertices.size() / 3;
    new_mesh->VerticesSize = verticesSize;
    new_mesh->Vertices = new float3[verticesSize]{};

    for (uint i = 0; i < verticesSize; i++)
    {
        new_mesh->Vertices[i].x = { vertices[3 * i] };
        new_mesh->Vertices[i].y = { vertices[(3 * i) + 1] };
        new_mesh->Vertices[i].z = { vertices[(3 * i) + 2] };

    }

    glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* verticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer
    
    //Indices
    uint indicesSize = indices.size();
    new_mesh->IndicesSize = indicesSize;
    new_mesh->Indices = new uint[new_mesh->IndicesSize]{};

    for (uint i = 0; i < indicesSize; i++)
    {
        new_mesh->Indices[i] = indices[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* new_mesh->IndicesSize, new_mesh->Indices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

    //Normals
    uint normalsSize = normals.size() / 3;
    new_mesh->NormalsSize = normalsSize;
    new_mesh->Normals = new float3[new_mesh->NormalsSize];


    for (uint i = 0; i < new_mesh->NormalsSize; ++i)
    {
        new_mesh->Normals[i].x = normals[3 * i];
        new_mesh->Normals[i].y = normals[(3 * i) + 1];
        new_mesh->Normals[i].z = normals[(3 * i) + 2];
    }

    //Texcoords
    uint texCoordsSize = texCoords.size();
    new_mesh->TexCoordsSize = texCoordsSize;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};
    
    for (uint i = 0; i < texCoordsSize; i++)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    return new_object;
}

GameObject* ModuleSceneManager::CreateCubeSphere(float radius, int sub, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

    ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    uint vertexCountPerRow;
    uint vertexCountPerFace;

    vertexCountPerRow = (unsigned int)pow(2, sub) + 1;
    vertexCountPerFace = vertexCountPerRow * vertexCountPerRow;

    if (smooth)
    {

    }
    else
    {
        // generate unit-length verties in +X face
        const float DEG2RAD = acos(-1) / 180.0f;

        std::vector<float> unitVertices;
        float n1[3];        // normal of longitudinal plane rotating along Y-axis
        float n2[3];        // normal of latitudinal plane rotating along Z-axis
        float v[3];         // direction vector intersecting 2 planes, n1 x n2
        float a1;           // longitudinal angle along y-axis
        float a2;           // latitudinal angle
        float scale;

        // rotate latitudinal plane from 45 to -45 degrees along Z-axis
        for (unsigned int i = 0; i < vertexCountPerRow; ++i)
        {
            // normal for latitudinal plane
            a2 = DEG2RAD * (45.0f - 90.0f * i / (vertexCountPerRow - 1));
            n2[0] = -sin(a2);
            n2[1] = cos(a2);
            n2[2] = 0;

            // rotate longitudinal plane from -45 to 45 along Y-axis
            for (unsigned int j = 0; j < vertexCountPerRow; ++j)
            {
                // normal for longitudinal plane
                a1 = DEG2RAD * (-45.0f + 90.0f * j / (vertexCountPerRow - 1));
                n1[0] = -sin(a1);
                n1[1] = 0;
                n1[2] = -cos(a1);

                // find direction vector of intersected line, n1 x n2
                v[0] = n1[1] * n2[2] - n1[2] * n2[1];
                v[1] = n1[2] * n2[0] - n1[0] * n2[2];
                v[2] = n1[0] * n2[1] - n1[1] * n2[0];

                // normalize direction vector
                scale = 1 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
                v[0] *= scale;
                v[1] *= scale;
                v[2] *= scale;

                unitVertices.push_back(v[0]);
                unitVertices.push_back(v[1]);
                unitVertices.push_back(v[2]);

                // DEBUG
                //std::cout << "vertex: (" << v[0] << ", " << v[1] << ", " << v[2] << "), "
                //          << sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) << std::endl;
            }
        }

        unsigned int k = 0, k1, k2, i1, i2; // indices
        float v1[3], v2[3], v3[3], v4[3];   // tmp vertices
        float t1[2], t2[2], t3[2], t4[2];   // texture coords
        float n[3];                         // normal vector

        // +X face
        for (unsigned int i = 0; i < vertexCountPerRow - 1; ++i)
        {
            k1 = i * vertexCountPerRow;              // index at curr row
            k2 = k1 + vertexCountPerRow;             // index at next row

            // vertical tex coords
            t1[1] = t3[1] = (float)i / (vertexCountPerRow - 1);
            t2[1] = t4[1] = (float)(i + 1) / (vertexCountPerRow - 1);

            for (unsigned int j = 0; j < vertexCountPerRow - 1; ++j, ++k1, ++k2)
            {
                i1 = k1 * 3;
                i2 = k2 * 3;

                // 4 vertices of a quad
                // v1--v3
                // | / |
                // v2--v4
                v1[0] = unitVertices[i1];
                v1[1] = unitVertices[i1 + 1];
                v1[2] = unitVertices[i1 + 2];
                v2[0] = unitVertices[i2];
                v2[1] = unitVertices[i2 + 1];
                v2[2] = unitVertices[i2 + 2];
                v3[0] = unitVertices[i1 + 3];
                v3[1] = unitVertices[i1 + 4];
                v3[2] = unitVertices[i1 + 5];
                v4[0] = unitVertices[i2 + 3];
                v4[1] = unitVertices[i2 + 4];
                v4[2] = unitVertices[i2 + 5];

                // compute face nornal
                const float EPSILON = 0.000001f;

                // default return value (0, 0, 0)
                n[0] = n[1] = n[2] = 0;

                // find 2 edge vectors: v1-v2, v1-v3
                float ex1 = v2[0] - v1[0];
                float ey1 = v2[1] - v1[1];
                float ez1 = v2[2] - v1[2];
                float ex2 = v3[0] - v1[0];
                float ey2 = v3[1] - v1[1];
                float ez2 = v3[2] - v1[2];

                // cross product: e1 x e2
                float nx, ny, nz;
                nx = ey1 * ez2 - ez1 * ey2;
                ny = ez1 * ex2 - ex1 * ez2;
                nz = ex1 * ey2 - ey1 * ex2;

                // normalize only if the length is > 0
                float length = sqrtf(nx * nx + ny * ny + nz * nz);
                if (length > EPSILON)
                {
                    // normalize
                    float lengthInv = 1.0f / length;
                    n[0] = nx * lengthInv;
                    n[1] = ny * lengthInv;
                    n[2] = nz * lengthInv;
                }

                // resize vertices by sphere radius
                v1[0] *= radius;
                v1[1] *= radius;
                v1[2] *= radius;

                v2[0] *= radius;
                v2[1] *= radius;
                v2[2] *= radius;

                v3[0] *= radius;
                v3[1] *= radius;
                v3[2] *= radius;

                v4[0] *= radius;
                v4[1] *= radius;
                v4[2] *= radius;

                // compute horizontal tex coords
                t1[0] = t2[0] = (float)j / (vertexCountPerRow - 1);
                t3[0] = t4[0] = (float)(j + 1) / (vertexCountPerRow - 1);

                // add 4 vertex attributes
                vertices.insert(vertices.end(), v1, v1 + 3);    // v1
                vertices.insert(vertices.end(), v2, v2 + 3);    // v2
                vertices.insert(vertices.end(), v3, v3 + 3);    // v3
                vertices.insert(vertices.end(), v4, v4 + 3);    // v4

                normals.insert(normals.end(), n, n + 3);  // n1
                normals.insert(normals.end(), n, n + 3);  // n2
                normals.insert(normals.end(), n, n + 3);  // n3
                normals.insert(normals.end(), n, n + 3);  // n4

                texCoords.insert(texCoords.end(), t1, t1 + 2);  // t1
                texCoords.insert(texCoords.end(), t2, t2 + 2);  // t2
                texCoords.insert(texCoords.end(), t3, t3 + 2);  // t3
                texCoords.insert(texCoords.end(), t4, t4 + 2);  // t4

                // add indices of 2 triangles
                indices.push_back(k);
                indices.push_back(k + 1);
                indices.push_back(k + 2);

                indices.push_back(k + 2);
                indices.push_back(k + 1);
                indices.push_back(k + 3);

                // add line indices; top and left
                //lineIndices.push_back(k);       // left
                //lineIndices.push_back(k + 1);
                //lineIndices.push_back(k);       // top
                //lineIndices.push_back(k + 2);

                k += 4;     // next
            }
        }

        // array size and index for building next face
        unsigned int startIndex;                    // starting index for next face
        int vertexSize = (int)vertices.size();      // vertex array size of +X face
        int indexSize = (int)indices.size();        // index array size of +X face
        //int lineIndexSize = (int)lineIndices.size(); // line index size of +X face

        // build -X face by negating x and z values
        startIndex = vertices.size() / 3;
        for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
        {
            vertices.push_back(-vertices[i]);
            vertices.push_back(vertices[i + 1]);
            vertices.push_back(-vertices[i + 2]);

            texCoords.push_back(texCoords[j]);
            texCoords.push_back(texCoords[j + 1]);

            normals.push_back(-normals[i]);
            normals.push_back(normals[i + 1]);
            normals.push_back(-normals[i + 2]);
        }
        for (int i = 0; i < indexSize; ++i)
        {
            indices.push_back(startIndex + indices[i]);
        }
        //for (int i = 0; i < lineIndexSize; i += 4)
        //{
        //    // left and bottom lines
        //    lineIndices.push_back(startIndex + i);      // left
        //    lineIndices.push_back(startIndex + i + 1);
        //    lineIndices.push_back(startIndex + i + 1);  // bottom
        //    lineIndices.push_back(startIndex + i + 3);
        //}

        // build +Y face by swapping x=>y, y=>-z, z=>-x
        startIndex = vertices.size() / 3;
        for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
        {
            vertices.push_back(-vertices[i + 2]);
            vertices.push_back(vertices[i]);
            vertices.push_back(-vertices[i + 1]);

            texCoords.push_back(texCoords[j]);
            texCoords.push_back(texCoords[j + 1]);

            normals.push_back(-normals[i + 2]);
            normals.push_back(normals[i]);
            normals.push_back(-normals[i + 1]);
        }
        for (int i = 0; i < indexSize; ++i)
        {
            indices.push_back(startIndex + indices[i]);
        }
        //for (int i = 0; i < lineIndexSize; ++i)
        //{
        //    // top and left lines (same as +X)
        //    lineIndices.push_back(startIndex + lineIndices[i]);
        //}

        // build -Y face by swapping x=>-y, y=>z, z=>-x
        startIndex = vertices.size() / 3;
        for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
        {
            vertices.push_back(-vertices[i + 2]);
            vertices.push_back(-vertices[i]);
            vertices.push_back(vertices[i + 1]);

            texCoords.push_back(texCoords[j]);
            texCoords.push_back(texCoords[j + 1]);

            normals.push_back(-normals[i + 2]);
            normals.push_back(-normals[i]);
            normals.push_back(normals[i + 1]);
        }
        for (int i = 0; i < indexSize; ++i)
        {
            indices.push_back(startIndex + indices[i]);
        }
        //for (int i = 0; i < lineIndexSize; i += 4)
        //{
        //    // top and right lines
        //    lineIndices.push_back(startIndex + i);      // top
        //    lineIndices.push_back(startIndex + i + 2);
        //    lineIndices.push_back(startIndex + 2 + i);  // right
        //    lineIndices.push_back(startIndex + 3 + i);
        //}

        // build +Z face by swapping x=>z, z=>-x
        startIndex = vertices.size() / 3;
        for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
        {
            vertices.push_back(-vertices[i + 2]);
            vertices.push_back(vertices[i + 1]);
            vertices.push_back(vertices[i]);

            texCoords.push_back(texCoords[j]);
            texCoords.push_back(texCoords[j + 1]);

            normals.push_back(-normals[i + 2]);
            normals.push_back(normals[i + 1]);
            normals.push_back(normals[i]);
        }
        for (int i = 0; i < indexSize; ++i)
        {
            indices.push_back(startIndex + indices[i]);
        }
        //for (int i = 0; i < lineIndexSize; ++i)
        //{
        //    // top and left lines (same as +X)
        //    lineIndices.push_back(startIndex + lineIndices[i]);
        //}

        // build -Z face by swapping x=>-z, z=>x
        startIndex = vertices.size() / 3;
        for (int i = 0, j = 0; i < vertexSize; i += 3, j += 2)
        {
            vertices.push_back(vertices[i + 2]);
            vertices.push_back(vertices[i + 1]);
            vertices.push_back(-vertices[i]);

            texCoords.push_back(texCoords[j]);
            texCoords.push_back(texCoords[j + 1]);

            normals.push_back(normals[i + 2]);
            normals.push_back(normals[i + 1]);
            normals.push_back(-normals[i]);
        }
        for (int i = 0; i < indexSize; ++i)
        {
            indices.push_back(startIndex + indices[i]);
        }
        //for (int i = 0; i < lineIndexSize; i += 4)
        //{
        //    // left and bottom lines
        //    lineIndices.push_back(startIndex + i);      // left
        //    lineIndices.push_back(startIndex + i + 1);
        //    lineIndices.push_back(startIndex + i + 1);  // bottom
        //    lineIndices.push_back(startIndex + i + 3);
        //}

        // generate interleaved vertex array as well
        /*std::vector<float>().swap(interleavedVertices);

        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            interleavedVertices.push_back(vertices[i]);
            interleavedVertices.push_back(vertices[i + 1]);
            interleavedVertices.push_back(vertices[i + 2]);

            interleavedVertices.push_back(normals[i]);
            interleavedVertices.push_back(normals[i + 1]);
            interleavedVertices.push_back(normals[i + 2]);

            interleavedVertices.push_back(texCoords[j]);
            interleavedVertices.push_back(texCoords[j + 1]);
        }*/
    }

    //Vertices
    uint verticesSize = vertices.size() / 3;
    new_mesh->VerticesSize = verticesSize;
    new_mesh->Vertices = new float3[verticesSize]{};

    for (uint i = 0; i < verticesSize; i++)
    {
        new_mesh->Vertices[i].x = { vertices[3 * i] };
        new_mesh->Vertices[i].y = { vertices[(3 * i) + 1] };
        new_mesh->Vertices[i].z = { vertices[(3 * i) + 2] };

    }

    glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * verticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    //Indices
    uint indicesSize = indices.size();
    new_mesh->IndicesSize = indicesSize;
    new_mesh->Indices = new uint[new_mesh->IndicesSize]{};

    for (uint i = 0; i < indicesSize; i++)
    {
        new_mesh->Indices[i] = indices[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->IndicesSize, new_mesh->Indices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

    //Normals
    uint normalsSize = normals.size() / 3;
    new_mesh->NormalsSize = normalsSize;
    new_mesh->Normals = new float3[new_mesh->NormalsSize];


    for (uint i = 0; i < new_mesh->NormalsSize; ++i)
    {
        new_mesh->Normals[i].x = normals[3 * i];
        new_mesh->Normals[i].y = normals[(3 * i) + 1];
        new_mesh->Normals[i].z = normals[(3 * i) + 2];
    }

    //Texcoords
    uint texCoordsSize = texCoords.size();
    new_mesh->TexCoordsSize = texCoordsSize;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};

    for (uint i = 0; i < texCoordsSize; i++)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    return new_object;
}

GameObject* ModuleSceneManager::CreateIcoSphere(float radius, int subdivision, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

    ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;

    if (smooth)
    {

    }
    else
    {
        const float S_STEP = 186 / 2048.0f;     // horizontal texture step
        const float T_STEP = 322 / 1024.0f;     // vertical texture step

        // compute 12 vertices of icosahedron
        const float PI = acos(-1);
        const float H_ANGLE = PI / 180 * 72;    // 72 degree = 360 / 5
        const float V_ANGLE = atanf(1.0f / 2);  // elevation = 26.565 degree

        std::vector<float> tmpVertices(12 * 3);    // 12 vertices
        int i1, i2;                             // indices
        float z, xy;                            // coords
        float hAngle1 = -PI / 2 - H_ANGLE / 2;  // start from -126 deg at 2nd row
        float hAngle2 = -PI / 2;                // start from -90 deg at 3rd row

        // the first top vertex (0, 0, r)
        tmpVertices[0] = 0;
        tmpVertices[1] = 0;
        tmpVertices[2] = radius;

        // 10 vertices at 2nd and 3rd rows
        for (int i = 1; i <= 5; ++i)
        {
            i1 = i * 3;         // for 2nd row
            i2 = (i + 5) * 3;   // for 3rd row

            z = radius * sinf(V_ANGLE);             // elevaton
            xy = radius * cosf(V_ANGLE);

            tmpVertices[i1] = xy * cosf(hAngle1);      // x
            tmpVertices[i2] = xy * cosf(hAngle2);
            tmpVertices[i1 + 1] = xy * sinf(hAngle1);  // x
            tmpVertices[i2 + 1] = xy * sinf(hAngle2);
            tmpVertices[i1 + 2] = z;                   // z
            tmpVertices[i2 + 2] = -z;

            // next horizontal angles
            hAngle1 += H_ANGLE;
            hAngle2 += H_ANGLE;
        }

        // the last bottom vertex (0, 0, -r)
        i1 = 11 * 3;
        tmpVertices[i1] = 0;
        tmpVertices[i1 + 1] = 0;
        tmpVertices[i1 + 2] = -radius;

        const float* v0, * v1, * v2, * v3, * v4, * v11;          // vertex positions
        float n[3];                                         // face normal
        float t0[2], t1[2], t2[2], t3[2], t4[2], t11[2];    // texCoords
        unsigned int index = 0;

        // compute and add 20 tiangles of icosahedron first
        v0 = &tmpVertices[0];       // 1st vertex
        v11 = &tmpVertices[11 * 3]; // 12th vertex
        for (int i = 1; i <= 5; ++i)
        {
            // 4 vertices in the 2nd row
            v1 = &tmpVertices[i * 3];
            if (i < 5)
                v2 = &tmpVertices[(i + 1) * 3];
            else
                v2 = &tmpVertices[3];

            v3 = &tmpVertices[(i + 5) * 3];
            if ((i + 5) < 10)
                v4 = &tmpVertices[(i + 6) * 3];
            else
                v4 = &tmpVertices[6 * 3];

            // texture coords
            t0[0] = (2 * i - 1) * S_STEP;   t0[1] = 0;
            t1[0] = (2 * i - 2) * S_STEP;   t1[1] = T_STEP;
            t2[0] = (2 * i - 0) * S_STEP;   t2[1] = T_STEP;
            t3[0] = (2 * i - 1) * S_STEP;   t3[1] = T_STEP * 2;
            t4[0] = (2 * i + 1) * S_STEP;   t4[1] = T_STEP * 2;
            t11[0] = 2 * i * S_STEP;         t11[1] = T_STEP * 3;

            const float EPSILON = 0.000001f;
            // add a triangle in 1st row

            // default return value (0, 0, 0)
            n[0] = n[1] = n[2] = 0;

            // find 2 edge vectors: v1-v2, v1-v3
            float ex1 = v1[0] - v0[0];
            float ey1 = v1[1] - v0[1];
            float ez1 = v1[2] - v0[2];
            float ex2 = v2[0] - v0[0];
            float ey2 = v2[1] - v0[1];
            float ez2 = v2[2] - v0[2];

            // cross product: e1 x e2
            float nx, ny, nz;
            nx = ey1 * ez2 - ez1 * ey2;
            ny = ez1 * ex2 - ex1 * ez2;
            nz = ex1 * ey2 - ey1 * ex2;

            // normalize only if the length is > 0
            float length = sqrtf(nx * nx + ny * ny + nz * nz);
            if (length > EPSILON)
            {
                // normalize
                float lengthInv = 1.0f / length;
                n[0] = nx * lengthInv;
                n[1] = ny * lengthInv;
                n[2] = nz * lengthInv;
            }

            vertices.push_back(v0[0]);  // x
            vertices.push_back(v0[1]);  // y
            vertices.push_back(v0[2]);  // z

            vertices.push_back(v1[0]);
            vertices.push_back(v1[1]);
            vertices.push_back(v1[2]);

            vertices.push_back(v2[0]);
            vertices.push_back(v2[1]);
            vertices.push_back(v2[2]);

            normals.push_back(n[0]);  // nx
            normals.push_back(n[1]);  // ny
            normals.push_back(n[2]);  // nz

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            texCoords.push_back(t0[0]); // s
            texCoords.push_back(t0[1]); // t

            texCoords.push_back(t1[0]);
            texCoords.push_back(t1[1]);

            texCoords.push_back(t2[0]);
            texCoords.push_back(t2[1]);

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);

            // add 2 triangles in 2nd row
            // default return value (0, 0, 0)
            n[0] = n[1] = n[2] = 0;

            // find 2 edge vectors: v1-v2, v1-v3
            ex1 = v3[0] - v1[0];
            ey1 = v3[1] - v1[1];
            ez1 = v3[2] - v1[2];
            ex2 = v2[0] - v1[0];
            ey2 = v2[1] - v1[1];
            ez2 = v2[2] - v1[2];

            // cross product: e1 x e2
            nx = ey1 * ez2 - ez1 * ey2;
            ny = ez1 * ex2 - ex1 * ez2;
            nz = ex1 * ey2 - ey1 * ex2;

            // normalize only if the length is > 0
            length = sqrtf(nx * nx + ny * ny + nz * nz);
            if (length > EPSILON)
            {
                // normalize
                float lengthInv = 1.0f / length;
                n[0] = nx * lengthInv;
                n[1] = ny * lengthInv;
                n[2] = nz * lengthInv;
            }

            vertices.push_back(v1[0]);  // x
            vertices.push_back(v1[1]);  // y
            vertices.push_back(v1[2]);  // z

            vertices.push_back(v3[0]);
            vertices.push_back(v3[1]);
            vertices.push_back(v3[2]);

            vertices.push_back(v2[0]);
            vertices.push_back(v2[1]);
            vertices.push_back(v2[2]);

            normals.push_back(n[0]);  // nx
            normals.push_back(n[1]);  // ny
            normals.push_back(n[2]);  // nz

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            texCoords.push_back(t1[0]); // s
            texCoords.push_back(t1[1]); // t

            texCoords.push_back(t3[0]);
            texCoords.push_back(t3[1]);

            texCoords.push_back(t2[0]);
            texCoords.push_back(t2[1]);

            indices.push_back(index + 3);
            indices.push_back(index + 4);
            indices.push_back(index + 5);

            // default return value (0, 0, 0)
            n[0] = n[1] = n[2] = 0;

            // find 2 edge vectors: v1-v2, v1-v3
            ex1 = v3[0] - v2[0];
            ey1 = v3[1] - v2[1];
            ez1 = v3[2] - v2[2];
            ex2 = v4[0] - v2[0];
            ey2 = v4[1] - v2[1];
            ez2 = v4[2] - v2[2];

            // cross product: e1 x e2
            nx = ey1 * ez2 - ez1 * ey2;
            ny = ez1 * ex2 - ex1 * ez2;
            nz = ex1 * ey2 - ey1 * ex2;

            // normalize only if the length is > 0
            length = sqrtf(nx * nx + ny * ny + nz * nz);
            if (length > EPSILON)
            {
                // normalize
                float lengthInv = 1.0f / length;
                n[0] = nx * lengthInv;
                n[1] = ny * lengthInv;
                n[2] = nz * lengthInv;
            }

            vertices.push_back(v2[0]);  // x
            vertices.push_back(v2[1]);  // y
            vertices.push_back(v2[2]);  // z

            vertices.push_back(v3[0]);
            vertices.push_back(v3[1]);
            vertices.push_back(v3[2]);

            vertices.push_back(v4[0]);
            vertices.push_back(v4[1]);
            vertices.push_back(v4[2]);

            normals.push_back(n[0]);  // nx
            normals.push_back(n[1]);  // ny
            normals.push_back(n[2]);  // nz

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            texCoords.push_back(t2[0]); // s
            texCoords.push_back(t2[1]); // t

            texCoords.push_back(t3[0]);
            texCoords.push_back(t3[1]);

            texCoords.push_back(t4[0]);
            texCoords.push_back(t4[1]);

            indices.push_back(index + 6);
            indices.push_back(index + 7);
            indices.push_back(index + 8);

            // add a triangle in 3rd row
            // default return value (0, 0, 0)
            n[0] = n[1] = n[2] = 0;

            // find 2 edge vectors: v1-v2, v1-v3
            ex1 = v11[0] - v3[0];
            ey1 = v11[1] - v3[1];
            ez1 = v11[2] - v3[2];
            ex2 = v4[0] - v3[0];
            ey2 = v4[1] - v3[1];
            ez2 = v4[2] - v3[2];

            // cross product: e1 x e2
            nx = ey1 * ez2 - ez1 * ey2;
            ny = ez1 * ex2 - ex1 * ez2;
            nz = ex1 * ey2 - ey1 * ex2;

            // normalize only if the length is > 0
            length = sqrtf(nx * nx + ny * ny + nz * nz);
            if (length > EPSILON)
            {
                // normalize
                float lengthInv = 1.0f / length;
                n[0] = nx * lengthInv;
                n[1] = ny * lengthInv;
                n[2] = nz * lengthInv;
            }

            vertices.push_back(v3[0]);  // x
            vertices.push_back(v3[1]);  // y
            vertices.push_back(v3[2]);  // z

            vertices.push_back(v11[0]);
            vertices.push_back(v11[1]);
            vertices.push_back(v11[12]);

            vertices.push_back(v4[0]);
            vertices.push_back(v4[1]);
            vertices.push_back(v4[2]);

            normals.push_back(n[0]);  // nx
            normals.push_back(n[1]);  // ny
            normals.push_back(n[2]);  // nz

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            normals.push_back(n[0]);
            normals.push_back(n[1]);
            normals.push_back(n[2]);

            texCoords.push_back(t3[0]); // s
            texCoords.push_back(t3[1]); // t

            texCoords.push_back(t11[0]);
            texCoords.push_back(t11[1]);

            texCoords.push_back(t4[0]);
            texCoords.push_back(t4[1]);

            indices.push_back(index + 9);
            indices.push_back(index + 10);
            indices.push_back(index + 11);

            //// add 6 edge lines per iteration
            ////  i
            ////  /   /   /   /   /       : (i, i+1)                              //
            //// /__ /__ /__ /__ /__                                              //
            //// \  /\  /\  /\  /\  /     : (i+3, i+4), (i+3, i+5), (i+4, i+5)    //
            ////  \/__\/__\/__\/__\/__                                            //
            ////   \   \   \   \   \      : (i+9,i+10), (i+9, i+11)               //
            ////    \   \   \   \   \                                             //
            //lineIndices.push_back(index);       // (i, i+1)
            //lineIndices.push_back(index + 1);       // (i, i+1)
            //lineIndices.push_back(index + 3);     // (i+3, i+4)
            //lineIndices.push_back(index + 4);
            //lineIndices.push_back(index + 3);     // (i+3, i+5)
            //lineIndices.push_back(index + 5);
            //lineIndices.push_back(index + 4);     // (i+4, i+5)
            //lineIndices.push_back(index + 5);
            //lineIndices.push_back(index + 9);     // (i+9, i+10)
            //lineIndices.push_back(index + 10);
            //lineIndices.push_back(index + 9);     // (i+9, i+11)
            //lineIndices.push_back(index + 11);

            // next index
            index += 12;
        }  
    }
    
    //Vertices
    uint verticesSize = vertices.size() / 3;
    new_mesh->VerticesSize = verticesSize;
    new_mesh->Vertices = new float3[verticesSize]{};

    for (uint i = 0; i < verticesSize; i++)
    {
        new_mesh->Vertices[i].x = { vertices[3 * i] };
        new_mesh->Vertices[i].y = { vertices[(3 * i) + 1] };
        new_mesh->Vertices[i].z = { vertices[(3 * i) + 2] };

    }

    glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * verticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    //Indices
    uint indicesSize = indices.size();
    new_mesh->IndicesSize = indicesSize;
    new_mesh->Indices = new uint[new_mesh->IndicesSize]{};

    for (uint i = 0; i < indicesSize; i++)
    {
        new_mesh->Indices[i] = indices[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->IndicesSize, new_mesh->Indices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

    //Normals
    uint normalsSize = normals.size() / 3;
    new_mesh->NormalsSize = normalsSize;
    new_mesh->Normals = new float3[new_mesh->NormalsSize];


    for (uint i = 0; i < new_mesh->NormalsSize; ++i)
    {
        new_mesh->Normals[i].x = normals[3 * i];
        new_mesh->Normals[i].y = normals[(3 * i) + 1];
        new_mesh->Normals[i].z = normals[(3 * i) + 2];
    }

    //Texcoords
    uint texCoordsSize = texCoords.size();
    new_mesh->TexCoordsSize = texCoordsSize;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};

    for (uint i = 0; i < texCoordsSize; i++)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    return new_object;
}

GameObject* ModuleSceneManager::CreateCylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth)
{
    GameObject* new_object = App->scene_manager->CreateEmptyGameObject();

    ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

    ResourceRenderer* Renderer = (ResourceRenderer*)new_object->AddResource(Res::ResType::Renderer);

    std::vector<float> unitCircleVertices;
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    uint baseIndex;                         // starting index of base
    uint topIndex;                          // starting index of top

    if (sectors < 3)
    {
        sectors = 3;
    }

    if (stacks < 1)
    {
        stacks = 1;
    }
    
    // generate unit circle vertices first
    const float PI = acos(-1);
    float sectorStep = 2 * PI / sectors;
    float sectorAngle;  // radian

    std::vector<float>().swap(unitCircleVertices);
    for (int i = 0; i <= sectors; ++i)
    {
        sectorAngle = i * sectorStep;
        unitCircleVertices.push_back(cos(sectorAngle)); // x
        unitCircleVertices.push_back(sin(sectorAngle)); // y
        unitCircleVertices.push_back(0);                // z
    }

    if (smooth)
    {
        //Cylinder with smooth vertices
    }
    else
    {
        // tmp vertex definition (x,y,z,s,t)
        struct Vertex
        {
            float x, y, z, s, t;
        };
        std::vector<Vertex> tmpVertices;

        int i, j, k;    // indices
        float x, y, z, s, t, radius;

        // put tmp vertices of cylinder side to array by scaling unit circle
        //NOTE: start and end vertex positions are same, but texcoords are different
        //      so, add additional vertex at the end point
        for (i = 0; i <= stacks; ++i)
        {
            z = -(height * 0.5f) + (float)i / stacks * height;      // vertex position z
            radius = baseRadius + (float)i / stacks * (topRadius - baseRadius);     // lerp
            t = 1.0f - (float)i / stacks;   // top-to-bottom

            for (j = 0, k = 0; j <= sectors; ++j, k += 3)
            {
                x = unitCircleVertices[k];
                y = unitCircleVertices[k + 1];
                s = (float)j / sectors;

                Vertex vertex;
                vertex.x = x * radius;
                vertex.y = y * radius;
                vertex.z = z;
                vertex.s = s;
                vertex.t = t;
                tmpVertices.push_back(vertex);
            }
        }

        Vertex v1, v2, v3, v4;      // 4 vertex positions v1, v2, v3, v4
        std::vector<float> n;       // 1 face normal
        int vi1, vi2;               // indices
        int index = 0;

        // v2-v4 <== stack at i+1
        // | \ |
        // v1-v3 <== stack at i
        for (i = 0; i < stacks; ++i)
        {
            vi1 = i * (sectors + 1);            // index of tmpVertices
            vi2 = (i + 1) * (sectors + 1);

            for (j = 0; j < sectors; ++j, ++vi1, ++vi2)
            {
                v1 = tmpVertices[vi1];
                v2 = tmpVertices[vi2];
                v3 = tmpVertices[vi1 + 1];
                v4 = tmpVertices[vi2 + 1];

                // compute a face normal of v1-v3-v2
                const float EPSILON = 0.000001f;

                std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
                float nx, ny, nz;

                // find 2 edge vectors: v1-v2, v1-v3
                float ex1 = v3.x - v1.x;
                float ey1 = v3.y - v1.y;
                float ez1 = v3.z - v1.z;
                float ex2 = v2.x - v1.x;
                float ey2 = v2.y - v1.y;
                float ez2 = v2.z - v1.z;

                // cross product: e1 x e2
                nx = ey1 * ez2 - ez1 * ey2;
                ny = ez1 * ex2 - ex1 * ez2;
                nz = ex1 * ey2 - ey1 * ex2;

                // normalize only if the length is > 0
                float length = sqrtf(nx * nx + ny * ny + nz * nz);
                if (length > EPSILON)
                {
                    // normalize
                    float lengthInv = 1.0f / length;
                    normal[0] = nx * lengthInv;
                    normal[1] = ny * lengthInv;
                    normal[2] = nz * lengthInv;
                }

                n = normal;

                // put quad vertices: v1-v2-v3-v4
                vertices.push_back(v1.x);
                vertices.push_back(v1.y);
                vertices.push_back(v1.z);

                vertices.push_back(v2.x);
                vertices.push_back(v2.y);
                vertices.push_back(v2.z);

                vertices.push_back(v3.x);
                vertices.push_back(v3.y);
                vertices.push_back(v3.z);

                vertices.push_back(v4.x);
                vertices.push_back(v4.y);
                vertices.push_back(v4.z);

                // put tex coords of quad
                texCoords.push_back(v1.s);
                texCoords.push_back(v1.t);

                texCoords.push_back(v2.s);
                texCoords.push_back(v2.t);

                texCoords.push_back(v3.s);
                texCoords.push_back(v3.t);

                texCoords.push_back(v4.s);
                texCoords.push_back(v4.t);

                // put normal
                for (k = 0; k < 4; ++k)  // same normals for all 4 vertices
                {
                    normals.push_back(n[0]);
                    normals.push_back(n[1]);
                    normals.push_back(n[2]);
                }

                // put indices of a quad
                indices.push_back(index);                       // v1-v3-v2
                indices.push_back(index + 2);
                indices.push_back(index + 1);

                indices.push_back(index + 1);                       // v2-v3-v4
                indices.push_back(index + 2);
                indices.push_back(index + 3);

                index += 4;     // for next
            }
        }

        // remember where the base index starts
        baseIndex = (unsigned int)indices.size();
        unsigned int baseVertexIndex = (unsigned int)vertices.size() / 3;

        // put vertices of base of cylinder
        z = -height * 0.5f;

        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(z);

        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(-1);

        texCoords.push_back(0.5f);
        texCoords.push_back(0.5f);

        for (i = 0, j = 0; i < sectors; ++i, j += 3)
        {
            x = unitCircleVertices[j];
            y = unitCircleVertices[j + 1];

            vertices.push_back(x * baseRadius);
            vertices.push_back(y * baseRadius);
            vertices.push_back(z);

            normals.push_back(0);
            normals.push_back(0);
            normals.push_back(-1);
            
            texCoords.push_back(-x * 0.5f + 0.5f);  // flip horizontal
            texCoords.push_back(-y * 0.5f + 0.5f);
        }

        // put indices for base
        for (i = 0, k = baseVertexIndex + 1; i < sectors; ++i, ++k)
        {
            if (i < sectors - 1)
            {
                indices.push_back(baseVertexIndex);
                indices.push_back(k + 1);
                indices.push_back(k);
            }
            else
            {
                indices.push_back(baseVertexIndex);
                indices.push_back(baseVertexIndex + 1);
                indices.push_back(k);
            }
        }

        // remember where the top index starts
        topIndex = (unsigned int)indices.size();
        unsigned int topVertexIndex = (unsigned int)vertices.size() / 3;

        // put vertices of top of cylinder
        z = height * 0.5f;

        vertices.push_back(0);
        vertices.push_back(0);
        vertices.push_back(z);

        normals.push_back(0);
        normals.push_back(0);
        normals.push_back(1);

        texCoords.push_back(0.5f);
        texCoords.push_back(0.5f);

        for (i = 0, j = 0; i < sectors; ++i, j += 3)
        {
            x = unitCircleVertices[j];
            y = unitCircleVertices[j + 1];

            vertices.push_back(x* topRadius);
            vertices.push_back(y* topRadius);
            vertices.push_back(z);

            normals.push_back(0);
            normals.push_back(0);
            normals.push_back(1);

            texCoords.push_back(x * 0.5f + 0.5f);
            texCoords.push_back(-y * 0.5f + 0.5f);
        }

        for (i = 0, k = topVertexIndex + 1; i < sectors; ++i, ++k)
        {
            if (i < sectors - 1)
            {
                indices.push_back(topVertexIndex);
                indices.push_back(k);
                indices.push_back(k + 1);
            } 
            else
            {
                indices.push_back(topVertexIndex);
                indices.push_back(k);
                indices.push_back(k + 1);
            }
        }

        // generate interleaved vertex array as well

        //std::vector<float>().swap(interleavedVertices);

        //std::size_t i, j;
        //std::size_t count = vertices.size();
        //for (i = 0, j = 0; i < count; i += 3, j += 2)
        //{
        //    //interleavedVertices.push_back(vertices[i]);
        //    //interleavedVertices.push_back(vertices[i+1]);
        //    //interleavedVertices.push_back(vertices[i+2]);
        //    interleavedVertices.insert(interleavedVertices.end(), &vertices[i], &vertices[i] + 3);

        //    //interleavedVertices.push_back(normals[i]);
        //    //interleavedVertices.push_back(normals[i+1]);
        //    //interleavedVertices.push_back(normals[i+2]);
        //    interleavedVertices.insert(interleavedVertices.end(), &normals[i], &normals[i] + 3);

        //    //interleavedVertices.push_back(texCoords[j]);
        //    //interleavedVertices.push_back(texCoords[j+1]);
        //    interleavedVertices.insert(interleavedVertices.end(), &texCoords[j], &texCoords[j] + 2);
    }

    //Vertices
    uint verticesSize = vertices.size() / 3;
    new_mesh->VerticesSize = verticesSize;
    new_mesh->Vertices = new float3[verticesSize]{};

    uint j = 0;
    for (uint i = 0; i < verticesSize; i++)
    {
        new_mesh->Vertices[i] = { vertices[j], vertices[j + 1], vertices[j + 2] };

        j = j + 3;
    }

    glGenBuffers(1, (GLuint*)&new_mesh->VerticesID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->VerticesID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * verticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    //Indices
    uint indicesSize = indices.size();
    new_mesh->IndicesSize = indicesSize;
    new_mesh->Indices = new uint[new_mesh->IndicesSize]{};

    for (uint i = 0; i < indicesSize; i++)
    {
        new_mesh->Indices[i] = indices[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->IndicesID); // create buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->IndicesID); // start using created buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->IndicesSize, new_mesh->Indices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

    //Normals
    uint normalsSize = normals.size() / 3;
    new_mesh->NormalsSize = normalsSize;
    new_mesh->Normals = new float3[new_mesh->NormalsSize];


    for (uint i = 0; i < new_mesh->NormalsSize; ++i)
    {
        new_mesh->Normals[i].x = normals[3 * i];
        new_mesh->Normals[i].y = normals[(3 * i) + 1];
        new_mesh->Normals[i].z = normals[(3 * i) + 2];
    }

    //TexCoords
    uint texCoordsSize = texCoords.size();
    new_mesh->TexCoordsSize = texCoordsSize;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};

    for (uint i = 0; i < texCoordsSize; i++)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

    glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

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
