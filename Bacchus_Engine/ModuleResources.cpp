#include "Application.h"
#include "ModuleResources.h"
#include "ResourceMesh.h"
#include "OpenGL.h"
#include "ModuleTextures.h"
#include "FileSystem.h"
#include "GameObject.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


#include "mmgr/mmgr.h"

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

bool ModuleResources::Init(json file)
{
	// --- Stream LOG messages to MyAssimpCallback, that sends them to console ---
	struct aiLogStream stream;
	stream.callback = MyAssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleResources::Start()
{
	LoadFBX("Assets/BH/BakerHouse.fbx");
	
	return true;
}


bool ModuleResources::CleanUp()
{
	//Detach assimp log stream
	aiDetachAllLogStreams();


	//Release all buffer data and own stored data
	for (uint i = 0; i < game_objects.size(); ++i)
	{
		delete game_objects[i];
	}

	return true;
}

bool ModuleResources::LoadFBX(const char* path)
{
	//Import scene from path
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);

	//Get Directory from filename
	std::string directory = path;
	App->fs->GetDirectoryFromPath(directory);

	//Query Scene first Material to get Texture name. If found, create a texture and return ID

	uint TextureID = 0;

	GetTextureIDFromSceneMaterial(*scene, TextureID, directory);

	if (scene != nullptr && scene->HasMeshes())
	{

		//Use scene->mNumMeshes to iterate on scene->mMeshes array

		for (uint i = 0; i < scene->mNumMeshes; ++i)
		{
			GameObject* new_object = new GameObject("GO");

			ResourceMesh* new_mesh = (ResourceMesh*)new_object->AddResource(Res::ResType::Mesh);

			
			game_objects.push_back(new_object);

			// Get Scene mesh number i
			aiMesh* mesh = scene->mMeshes[i];

			// Import mesh data (fill new_mesh)
			new_mesh->ImportMesh(mesh, TextureID);
		}

		// Free scene
		aiReleaseImport(scene);


	}
	else
		LOG("|[error]: Error loading scene %s", path);


	return true;
}


void ModuleResources::Draw() const
{
		

	for (uint i = 0; i < game_objects.size(); ++i)
	{
		ResourceMesh* mesh = (ResourceMesh*)game_objects[i]->GetResource(Res::ResType::Mesh);

		if (mesh)
		{
			DrawMesh(mesh);
			DrawNormals(mesh);
		}
		else
		{
			LOG("|[error]: Could not find Mesh component in current game object");
		}
	}
}

uint ModuleResources::GetNumMeshes() const
{
	return game_objects.size();
}

void ModuleResources::GetTextureIDFromSceneMaterial(const aiScene& scene, uint& texture_ID, std::string& directory)
{
	if (scene.HasMaterials())
	{
		// --- Get scene's first material ---
		aiMaterial* material = scene.mMaterials[0];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Texture_path;

			// --- Specify type of texture to retrieve (in this case DIFFUSE/ALBEDO)---
			material->GetTexture(aiTextureType_DIFFUSE, 0, &Texture_path);

			// --- Build whole path to texture file ---
			directory.append(Texture_path.C_Str());

			// --- If we find the texture file, load it ---
			if (App->fs->Exists(directory.data()))
				texture_ID = App->tex->CreateTextureFromFile(directory.data());

		}
	}
}

void ModuleResources::DrawMesh(const ResourceMesh* mesh) const
{
	// --- Draw Texture ---
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mesh->TextureID);
	glActiveTexture(GL_TEXTURE0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->TextureCoordsID);
	glTexCoordPointer(2, GL_FLOAT, 0, NULL);

	// --- Draw mesh ---
	glEnableClientState(GL_VERTEX_ARRAY); // enable client-side capability

	glBindBuffer(GL_ARRAY_BUFFER, mesh->VerticesID); // start using created buffer (vertices)
	glVertexPointer(3, GL_FLOAT, 0, NULL); // Use selected buffer as vertices 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IndicesID); // start using created buffer (indices)
	glDrawElements(GL_TRIANGLES, mesh->IndicesSize, GL_UNSIGNED_INT, NULL); // render primitives from array data

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer (vertices)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer (indices)

	glDisableClientState(GL_VERTEX_ARRAY); // disable client-side capability

	// ----        ----

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleResources::DrawNormals(const ResourceMesh* mesh) const
{
	// --- Draw Vertex Normals ---
	if (mesh->Normals)
	{
		glBegin(GL_LINES);
		glLineWidth(1.0f);

		glColor4f(0.0f, 0.5f, 0.5f, 1.0f);

		for (uint j = 0; j < mesh->VerticesSize; ++j)
		{
			glVertex3f(mesh->Vertices[j].x, mesh->Vertices[j].y, mesh->Vertices[j].z);
			glVertex3f(mesh->Vertices[j].x + mesh->Normals[j].x * NORMAL_LENGTH, mesh->Vertices[j].y + mesh->Normals[j].y * NORMAL_LENGTH, mesh->Vertices[j].z + mesh->Normals[j].z * NORMAL_LENGTH);
		}

		// --- Draw Face Normals 

		Triangle face;

		for (uint j = 0; j < mesh->VerticesSize / 3; ++j)
		{
			face.a = mesh->Vertices[(j * 3) + 2];
			face.b = mesh->Vertices[(j * 3) + 1];
			face.c = mesh->Vertices[(j * 3)];

			float3 face_center = face.Centroid();
			float3 face_normal = face.NormalCW();

			glVertex3f(face_center.x, face_center.y, face_center.z);
			glVertex3f(face_center.x + face_normal.x * NORMAL_LENGTH, face_center.y + face_normal.y * NORMAL_LENGTH, face_center.z + face_normal.z * NORMAL_LENGTH);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();

	}

}
