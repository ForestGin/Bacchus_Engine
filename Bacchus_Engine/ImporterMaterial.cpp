#include "ImporterMaterial.h"
#include "Application.h"
#include "FileSystem.h"
#include "ModuleTextures.h"

#include "ResourceMaterial.h"

#include "Assimp/include/scene.h"

#include "mmgr/mmgr.h"

ImporterMaterial::ImporterMaterial()
{
}

ImporterMaterial::~ImporterMaterial()
{
}

bool ImporterMaterial::Import(const char& File_path, const ImportData& IData) const
{
	ImportMaterialData MData = (ImportMaterialData&)IData;

	std::string directory = &File_path;
	App->fs->GetDirectoryFromPath(directory);

	if (MData.scene->HasMaterials())
	{
		
		aiMaterial* material = MData.scene->mMaterials[0];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Texture_path;

			
			material->GetTexture(aiTextureType_DIFFUSE, 0, &Texture_path);

			
			directory.append(Texture_path.C_Str());

		
			if (App->fs->Exists(directory.data()))
				MData.new_material->TextureID = App->tex->CreateTextureFromFile(directory.data());

		}
	}

	return true;
}