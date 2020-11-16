#include "ImporterMesh.h"
#include "ComponentMesh.h"
#include "Application.h"
#include "FileSystem.h"
#include "ModuleRenderer3D.h"


#include "Assimp/include/scene.h"

#include "OpenGL.h"
#include "Math.h"

#include "mmgr/mmgr.h"

ImporterMesh::ImporterMesh()
{
}

ImporterMesh::~ImporterMesh()
{
}

bool ImporterMesh::Import(const ImportData& IData) const
{

	ImportMeshData data = (ImportMeshData&)IData;

	
	data.new_mesh->VerticesSize = data.mesh->mNumVertices;
	data.new_mesh->Vertices = new float3[data.mesh->mNumVertices];

	for (uint i = 0; i < data.mesh->mNumVertices; ++i)
	{
		data.new_mesh->Vertices[i].x = data.mesh->mVertices[i].x;
		data.new_mesh->Vertices[i].y = data.mesh->mVertices[i].y;
		data.new_mesh->Vertices[i].z = data.mesh->mVertices[i].z;
	}

	glGenBuffers(1, (GLuint*)&data.new_mesh->VerticesID); // create buffer
	glBindBuffer(GL_ARRAY_BUFFER, data.new_mesh->VerticesID); // start using created buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * data.new_mesh->VerticesSize, data.new_mesh->Vertices, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer
	

	if (data.mesh->HasNormals())
	{
		data.new_mesh->NormalsSize = data.mesh->mNumVertices;
		data.new_mesh->Normals = new float3[data.new_mesh->NormalsSize];

		for (uint i = 0; i < data.mesh->mNumVertices; ++i)
		{
			data.new_mesh->Normals[i].x = data.mesh->mNormals[i].x;
			data.new_mesh->Normals[i].y = data.mesh->mNormals[i].y;
			data.new_mesh->Normals[i].z = data.mesh->mNormals[i].z;
		}

	}

	if (data.mesh->HasTextureCoords(0))
	{
		data.new_mesh->TexCoords = new float[data.mesh->mNumVertices * 2];

		for (uint j = 0; j < data.mesh->mNumVertices; ++j)
		{
			data.new_mesh->TexCoords[j * 2] = data.mesh->mTextureCoords[0][j].x;
			data.new_mesh->TexCoords[(j * 2) + 1] = data.mesh->mTextureCoords[0][j].y;
		}
		
	}

	glGenBuffers(1, (GLuint*)&data.new_mesh->TextureCoordsID); // create buffer
	glBindBuffer(GL_ARRAY_BUFFER, data.new_mesh->TextureCoordsID); // start using created buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.new_mesh->VerticesSize * 2, data.new_mesh->TexCoords, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer


	data.new_mesh->IndicesSize = data.mesh->mNumFaces * 3;
	data.new_mesh->Indices = new uint[data.new_mesh->IndicesSize];

	for (unsigned j = 0; j < data.mesh->mNumFaces; ++j)
	{
		const aiFace& face = data.mesh->mFaces[j];

		if (face.mNumIndices > 3)
		{
			LOG("|[error]: Importer Mesh found a quad in %s, ignoring it. ", data.mesh->mName);
			continue;
		}

		data.new_mesh->Indices[j * 3] = face.mIndices[0];
		data.new_mesh->Indices[j * 3 + 1] = face.mIndices[1];
		data.new_mesh->Indices[j * 3 + 2] = face.mIndices[2];
	}

	glGenBuffers(1, (GLuint*)&data.new_mesh->IndicesID); // create buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.new_mesh->IndicesID); // start using created buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * data.new_mesh->IndicesSize, data.new_mesh->Indices, GL_STATIC_DRAW); // send vertices to VRAM
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer

	return true;
}

void ImporterMesh::Save(ComponentMesh* mesh, const char* path) const
{
	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[2] = { mesh->IndicesSize, mesh->VerticesSize };

	uint size = sizeof(ranges) + sizeof(uint) * mesh->IndicesSize + sizeof(float) * mesh->VerticesSize * 3;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);

	cursor += bytes; // Store indices
	bytes = sizeof(uint) * mesh->IndicesSize;
	memcpy(cursor, mesh->Indices, bytes);	

	cursor += bytes; // Store vertices
	bytes = sizeof(float3) * mesh->VerticesSize;
	memcpy(cursor, mesh->Vertices, bytes);

	App->fs->Save(path, data, size);
}
