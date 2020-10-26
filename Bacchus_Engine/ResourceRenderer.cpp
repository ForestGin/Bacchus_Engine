#include "ResourceRenderer.h"

#include "mmgr/mmgr.h"

ResourceRenderer::ResourceRenderer(GameObject* ContainerGO) : Res(ContainerGO, Res::ResType::Renderer)
{
}

ResourceRenderer::~ResourceRenderer()
{
}