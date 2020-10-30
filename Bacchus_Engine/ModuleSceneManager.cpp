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
	DefaultMaterial = CreateEmptyMaterial();
	DefaultMaterial->Texture_path = "Default";

	CheckersMaterial = CreateEmptyMaterial();
	CheckersMaterial->TextureID = App->tex->GetCheckerTextureID();
	CheckersMaterial->Texture_path = "NONE";
	CheckersMaterial->Texture_width = CHECKERS_WIDTH;
	CheckersMaterial->Texture_height = CHECKERS_HEIGHT;

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
	ResourceMaterial* Material = (ResourceMaterial*)game_objects[SelectedGameObject]->GetResource(Res::ResType::Material);

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

	new_object->SetMaterial(DefaultMaterial);

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

    /*if (checkers)
    {
        new_object->SetMaterial(CheckersMaterial);
    }*/

	glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

	return new_object;
}

GameObject* ModuleSceneManager::CreateSphere(float radius, int sectors, int stacks, bool smooth, bool checkers)
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

    //Converting "vertices" to new_mesh->Vertices"
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float3)* verticesSize, new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer
    
    //Converting "indices" to "new_mesh->Vertices"
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

    //Converting "texCoords" to "new_mesh->Vertices"
    uint texCoordsSize = texCoords.size();
    new_mesh->TexCoordsSize = texCoordsSize;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};
    
    for (uint i = 0; i < texCoordsSize; i++)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

   /* if (checkers)
    {
        new_object->SetMaterial(CheckersMaterial);
    }*/

    glGenBuffers(1, (GLuint*)&new_mesh->TextureCoordsID); // create buffer
    glBindBuffer(GL_ARRAY_BUFFER, new_mesh->TextureCoordsID); // start using created buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->TexCoordsSize, new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer

    return new_object;
}

GameObject* ModuleSceneManager::CreateCylinder(float baseRadius, float topRadius, float height, int sectors, int stacks, bool smooth, bool checkers)
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

    //Converting "vertices" to new_mesh->Vertices"
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

    //Converting "indices" to "new_mesh->Vertices"
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

    //Converting "texCoords" to "new_mesh->Vertices"
    uint texCoordsSize = texCoords.size();
    new_mesh->TexCoordsSize = texCoordsSize;
    new_mesh->TexCoords = new float[new_mesh->TexCoordsSize]{};

    for (uint i = 0; i < texCoordsSize; i++)
    {
        new_mesh->TexCoords[i] = texCoords[i];
    }

    /*if (checkers)
    {
        new_object->SetMaterial(CheckersMaterial);
    }*/

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
