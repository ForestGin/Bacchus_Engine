#ifndef __IMPORTER_SCENE_H__
#define __IMPORTER_SCENE_H__

#include "Importer.h"
#include <vector>

class ImporterMesh;
class ImporterMaterial;
struct aiNode;
struct aiScene;
class ResourceMaterial;
class GameObject;


struct ImportSceneData : public ImportData
{

};

class ImporterScene : public Importer
{

public:
	ImporterScene();
	virtual ~ImporterScene();

	bool Import(const char* File_path, const ImportData& IData) const override;
	bool Load(const char* exported_file) const override;
	void SaveSceneToFile(std::vector<GameObject*>& scene_gos, std::string& scene_name) const;

private:
	void LoadNodes(const aiNode* node, GameObject* parent, const aiScene* scene, ResourceMaterial* Material, std::vector<GameObject*>& scene_gos) const;
	ImporterMesh* IMesh = nullptr;
	ImporterMaterial* IMaterial = nullptr;
};

#endif
