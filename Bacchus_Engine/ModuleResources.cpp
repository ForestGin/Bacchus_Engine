#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"

#include "OpenGL.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


void MyAssimpCallback(const char* msg, char* userData)
{
	LOG("[Assimp]: %s", msg);
}

bool ModuleResources::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();


	for (uint i = 0; i < meshes.size(); ++i)
	{
		glDeleteBuffers(1, (GLuint*)&meshes[i]->VBO);
		glDeleteBuffers(1, (GLuint*)&meshes[i]->IBO);

		RELEASE_ARRAY(meshes[i]->Vertices);
		RELEASE_ARRAY(meshes[i]->Indices);

		delete meshes[i];
	}

	return true;
}

bool ModuleResources::LoadFBX(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		// Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			// --- Create new Resource mesh to store current scene mesh data ---
			ResourceMesh* new_mesh = new ResourceMesh;
			meshes.push_back(new_mesh);

			// --- Get Scene mesh number i ---
			aiMesh* mesh = scene->mMeshes[i];

			// --- Import mesh data (fill new_mesh)---
			new_mesh->ImportMesh(mesh);
		}
		aiReleaseImport(scene);


	}
	else
		LOG("|[error]: Error loading scene %s", path);


	return true;
}

ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init(/*json file*/)
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleResources::Start()
{
	//LoadFile("Assets/warrior.fbx");

	return true;
}

void ModuleResources::Draw()
{
	

	for (uint i = 0; i < meshes.size(); ++i)
	{
		glEnableClientState(GL_VERTEX_ARRAY); // enable client-side capability

		// --- SHOULD CREATE DRAW FUNCTION IN RENDERER TO DRAW MESHES ---
		glBindVertexArray(meshes[i]->VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i]->IBO);

		glDrawElements(GL_TRIANGLES, meshes[i]->IndicesSize, GL_UNSIGNED_INT, NULL);

		/*glBindBuffer(GL_ARRAY_BUFFER, 0);*/
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glDisableClientState(GL_VERTEX_ARRAY); // disable client-side capability


		

		if (meshes[i]->Vertices->normal)
		{
			glBegin(GL_LINES);
			glLineWidth(1.0f);
			uint Normal_length = 1;

			glColor4f(0.0f, 0.5f, 0.5f, 1.0f);

			for (uint j = 0; j < meshes[i]->verticesSize; ++j)
			{
				glVertex3f(meshes[i]->Vertices[j].position[0], meshes[i]->Vertices[j].position[1], meshes[i]->Vertices[j].position[2]);
				glVertex3f(meshes[i]->Vertices[j].position[0] + meshes[i]->Vertices[j].normal[0] * Normal_length, meshes[i]->Vertices[j].position[1] + meshes[i]->Vertices[j].normal[1] * Normal_length, meshes[i]->Vertices[j].position[2] + meshes[i]->Vertices[j].normal[2] * Normal_length);
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			glEnd();

		}

	}


}