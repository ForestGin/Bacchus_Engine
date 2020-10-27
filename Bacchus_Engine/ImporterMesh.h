#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"

struct aiMesh;
class ResourceMesh;

struct ImportMeshData : public ImportData
{
	aiMesh* mesh = nullptr;
	ResourceMesh* new_mesh = nullptr;
};

class ImporterMesh : public Importer
{

public:
	ImporterMesh();
	virtual ~ImporterMesh();

	bool Import(const ImportData& IData) const override;
};

#endif
