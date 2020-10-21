#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "OpenGL.h"
#include "ModuleTextures.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")



void MyAssimpCallback(const char* msg, char* userData)
{
	LOG("[Assimp]: %s", msg);
}

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init(/*json file*/)
{
	// --- Stream LOG messages to MyAssimpCallback, that sends them to console ---
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleResources::Start()
{
	LoadFBX("Assets/BakerHouse.fbx");
	HouseTexID = App->tex->CreateTextureFromFile("Assets/Lenna.png");

	return true;
}


bool ModuleResources::CleanUp()
{
	//Detach assimp log stream
	aiDetachAllLogStreams();


	//Release all buffer data and own stored data
	for (uint i = 0; i < meshes.size(); ++i)
	{
		delete meshes[i];
	}

	//Delete sample Image texture
	if (HouseTexID > 0)
		glDeleteTextures(1, (GLuint*)&HouseTexID);

	return true;
}

bool ModuleResources::LoadFBX(const char* path)
{
	//Import scene from path
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		//Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			//Create new Resource mesh to store current scene mesh data
			ResourceMesh* new_mesh = new ResourceMesh;
			meshes.push_back(new_mesh);

			// Get Scene mesh number i
			aiMesh* mesh = scene->mMeshes[i];

			// Import mesh data (fill new_mesh)
			new_mesh->ImportMesh(mesh);
		}

		// Free scene
		aiReleaseImport(scene);


	}
	else
		LOG("|[error]: Error loading scene %s", path);


	return true;
}


void ModuleResources::Draw()
{
		

	for (uint i = 0; i < meshes.size(); ++i)
	{

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, HouseTexID);
		glActiveTexture(GL_TEXTURE0);

		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->TextureCoordsID);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);

		// Drawing mesh
		glEnableClientState(GL_VERTEX_ARRAY); // enable client-side capability

		glBindBuffer(GL_ARRAY_BUFFER, meshes[i]->VerticesID); // start using created buffer (vertices)
		glVertexPointer(3, GL_FLOAT, 0, NULL); // Use selected buffer as vertices 

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->IndicesID); // start using created buffer (indices)
		glDrawElements(GL_TRIANGLES, meshes[i]->IndicesSize, GL_UNSIGNED_INT, NULL); // render primitives from array data

		glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer (vertices)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer (indices)

		glDisableClientState(GL_VERTEX_ARRAY); // disable client-side capability

		//
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Drawing Vertex Normals
		/*if (meshes[i]->Normals)
		{
			glBegin(GL_LINES);
			glLineWidth(1.0f);
			uint Normal_length = 1;

			glColor4f(0.0f, 0.5f, 0.5f, 1.0f);

			for (uint j = 0; j < meshes[i]->VerticesSize; ++j)
			{
				glVertex3f(meshes[i]->Vertices[j].x, meshes[i]->Vertices[j].y, meshes[i]->Vertices[j].z);
				glVertex3f(meshes[i]->Vertices[j].x + meshes[i]->Normals[j].x * Normal_length, meshes[i]->Vertices[j].y + meshes[i]->Normals[j].y * Normal_length, meshes[i]->Vertices[j].z + meshes[i]->Normals[j].z * Normal_length);
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glEnd();

		}*/


	}
}

uint ModuleResources::GetNumMeshes() const
{
	return meshes.size();
}