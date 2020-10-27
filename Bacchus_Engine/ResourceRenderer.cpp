#include "ResourceRenderer.h"
#include "ResourceMaterial.h"
#include "ResourceMesh.h"
#include "GameObject.h"
#include "OpenGL.h"

#include "mmgr/mmgr.h"

ResourceRenderer::ResourceRenderer(GameObject* ContainerGO) : Res(ContainerGO, Res::ResType::Renderer)
{
}

ResourceRenderer::~ResourceRenderer()
{
}

void ResourceRenderer::Draw()
{
	ResourceMesh* mesh = (ResourceMesh*)this->GO->GetResource(Res::ResType::Mesh);

	if (mesh)
	{
		DrawMesh(*mesh);
		/*DrawNormals(*mesh);*/
	}
}

void ResourceRenderer::DrawMesh(ResourceMesh& mesh) const
{

	//Draw Texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY); // enable gl capability
	glEnableClientState(GL_VERTEX_ARRAY); // enable client-side capability
	glEnable(GL_TEXTURE_2D); // enable gl capability
	glActiveTexture(GL_TEXTURE0); // In case we had multitexturing, we should set which one is active 

	//If the mesh has a material associated, get it
	ResourceMaterial* mat = (ResourceMaterial*)mesh.GetContainerGameObject()->GetResource(Res::ResType::Material);

	if (mat)
	{
		glBindTexture(GL_TEXTURE_2D, mat->TextureID); // start using texture
		glBindBuffer(GL_ARRAY_BUFFER, mesh.TextureCoordsID); // start using created buffer (tex coords)
		glTexCoordPointer(2, GL_FLOAT, 0, NULL); // Specify type of data format
	}

	//Draw mesh
	glBindBuffer(GL_ARRAY_BUFFER, mesh.VerticesID); // start using created buffer (vertices)
	glVertexPointer(3, GL_FLOAT, 0, NULL); // Use selected buffer as vertices 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IndicesID); // start using created buffer (indices)
	glDrawElements(GL_TRIANGLES, mesh.IndicesSize, mesh.IndexDatatype, NULL); // render primitives from array data



	//Unbind buffers, Disable capabilities

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Stop using buffer (vertices)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Stop using buffer (indices)
	glBindTexture(GL_TEXTURE_2D, 0); // Stop using buffer (texture)

	glDisable(GL_TEXTURE_2D); // enable gl capability
	glActiveTexture(GL_TEXTURE0); // In case we had multitexturing, we should reset active texture
	glDisableClientState(GL_VERTEX_ARRAY); // disable client-side capability
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); // disable client-side capability

}

void ResourceRenderer::DrawNormals(const ResourceMesh& mesh) const
{
	//Draw Mesh Normals

	if (mesh.Normals)
	{
		//Draw Vertex Normals

		glBegin(GL_LINES);
		glLineWidth(1.0f);

		glColor4f(0.0f, 0.5f, 0.5f, 1.0f);

		
	//Draw Face Normals 

		Triangle face;

		for (uint j = 0; j < mesh.VerticesSize / 3; ++j)
		{
			face.a = mesh.Vertices[(j * 3)];
			face.b = mesh.Vertices[(j * 3) + 1];
			face.c = mesh.Vertices[(j * 3) + 2];

			float3 face_center = face.Centroid();
			
			float3 face_normal = Cross(face.a - face.b, face.c - face.b);

			face_normal.Normalize();

			glVertex3f(face_center.x, face_center.y, face_center.z);
			glVertex3f(face_center.x + face_normal.x * NORMAL_LENGTH, face_center.y + face_normal.y * NORMAL_LENGTH, face_center.z + face_normal.z * NORMAL_LENGTH);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnd();

	}

}