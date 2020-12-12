#include "ImporterMaterial.h"
#include "Application.h"
#include "FileSystem.h"
#include "ModuleTextures.h"
#include "ModuleResources.h"

#include "ComponentMaterial.h"
#include "ResourceMaterial.h"

#include "Assimp/include/scene.h"

#include "mmgr/mmgr.h"

ImporterMaterial::ImporterMaterial()
{
}

ImporterMaterial::~ImporterMaterial()
{
}

bool ImporterMaterial::Import(const char* File_path, const ImportData& IData) const
{
	ImportMaterialData MData = (ImportMaterialData&)IData;

	//Get Directory from filename
	std::string directory = File_path;
	App->fs->GetDirectoryFromPath(directory);

	if (MData.scene->HasMaterials())
	{
		//Get scene's first material
		aiMaterial* material = MData.scene->mMaterials[MData.mesh->mMaterialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString Texture_path;

			//Specify type of texture to retrieve (in this case DIFFUSE/ALBEDO)---
			material->GetTexture(aiTextureType_DIFFUSE, 0, &Texture_path);

			std::string final_path = Texture_path.C_Str();
			App->fs->SplitFilePath(final_path.data(), nullptr, &final_path);

			//Build whole path to texture file
			directory.append(final_path);

			//Duplicate texture in assets folder
			std::string assetpath = ASSETS_FOLDER;
			assetpath.append(final_path);

			if (!App->fs->Exists(assetpath.data()))
				App->fs->CopyFromOutsideFS(directory.data(), assetpath.data());

			//If we find the texture file, load it
			ResourceTexture* texture = (ResourceTexture*)App->res->GetResource(directory.data());

			if (texture)
			{
				MData.new_material->resource_diffuse = texture;
				texture->instances++;
			}
			else
			{
				MData.new_material->resource_diffuse = (ResourceTexture*)App->res->CreateResource(Resource::ResourceType::TEXTURE);
				MData.new_material->resource_diffuse->buffer_id = App->tex->CreateTextureFromFile(directory.data(), MData.new_material->resource_diffuse->Texture_width, MData.new_material->resource_diffuse->Texture_height, MData.new_material->resource_diffuse->GetUID());
				MData.new_material->resource_diffuse->SetOriginalFilename(directory.data());
				MData.new_material->resource_diffuse->Texture_path = directory.data();
				App->res->CreateMetaFromUID(MData.new_material->resource_diffuse->GetUID(), assetpath.data());
			}
		}
	}

	return true;
}

void ImporterMaterial::Load(const char* filename, ResourceMaterial& mat)
{
	mat.resource_diffuse = (ResourceTexture*)App->res->CreateResource(Resource::ResourceType::TEXTURE);
	mat.resource_diffuse->buffer_id = App->tex->CreateTextureFromFile(filename, mat.resource_diffuse->Texture_width, mat.resource_diffuse->Texture_height, mat.resource_diffuse->GetUID());
	mat.resource_diffuse->Texture_path = filename;
	mat.resource_diffuse->SetOriginalFilename(filename);
}