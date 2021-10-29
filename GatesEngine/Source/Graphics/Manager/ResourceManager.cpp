#include "..\..\..\Header\Graphics\Manager\ResourceManager.h"
#include "..\..\..\Header\Graphics\Manager\TextureManager.h"
#include "..\..\..\Header\Graphics\Manager\ShaderManager.h"
#include "..\..\..\Header\Graphics\Manager\MeshManager.h"

GatesEngine::TextureManager* GatesEngine::ResourceManager::textureManager = nullptr;
GatesEngine::ShaderManager* GatesEngine::ResourceManager::shaderManager = nullptr;
GatesEngine::MeshManager* GatesEngine::ResourceManager::meshManager = nullptr;
bool GatesEngine::ResourceManager::isInitialize = false;

void GatesEngine::ResourceManager::Initialize(GraphicsDevice* gDevice)
{
	if (isInitialize)return;
	isInitialize = true;
	textureManager = new TextureManager(gDevice);
	shaderManager = new ShaderManager(gDevice);
	meshManager = new MeshManager(gDevice);
}

void GatesEngine::ResourceManager::Terminate()
{
	delete textureManager;
	delete shaderManager;
	delete meshManager;

	isInitialize = false;
}

GatesEngine::TextureManager* GatesEngine::ResourceManager::GetTextureManager()
{
	return textureManager;
}

GatesEngine::ShaderManager* GatesEngine::ResourceManager::GetShaderManager()
{
	return shaderManager;
}

GatesEngine::MeshManager* GatesEngine::ResourceManager::GetMeshManager()
{
	return meshManager;
}
