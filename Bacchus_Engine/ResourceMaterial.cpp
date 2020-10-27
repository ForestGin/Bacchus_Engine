#include "ResourceMaterial.h"
#include "Application.h"
#include "FileSystem.h"
#include "ModuleTextures.h"
#include "OpenGL.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "mmgr/mmgr.h"


ResourceMaterial::ResourceMaterial(Res::ResType type) : Res(type)
{
}

ResourceMaterial::~ResourceMaterial()
{
	glDeleteBuffers(1, (GLuint*)&TextureID);
}

void ResourceMaterial::CreateEmpty()
{
	
}


void ResourceMaterial::CreateFromScene(const aiScene* scene, const char* file_path)
{
	
	std::string directory = file_path;
	App->fs->GetDirectoryFromPath(directory);

	if (scene->HasMaterials())
	{
		
		aiMaterial* material = scene->mMaterials[0];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Texture_path;

			
			material->GetTexture(aiTextureType_DIFFUSE, 0, &Texture_path);

			
			directory.append(Texture_path.C_Str());

			
			if (App->fs->Exists(directory.data()))
				TextureID = App->tex->CreateTextureFromFile(directory.data());

		}
	}

}