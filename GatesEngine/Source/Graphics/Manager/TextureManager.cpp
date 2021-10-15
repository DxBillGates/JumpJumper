#include "..\..\..\Header\Graphics\Manager\TextureManager.h"
#include "..\..\..\Header\Graphics\GraphicsDevice.h"
#include "..\..\..\Header\Graphics\Texture.h"

GatesEngine::TextureManager::TextureManager(GraphicsDevice* pGraphicsDevice)
	: graphicsDevice(pGraphicsDevice)
{
}

GatesEngine::TextureManager::~TextureManager()
{
	for (auto t : textures)
	{
		delete t.second;
	}
}

GatesEngine::Texture* GatesEngine::TextureManager::Add(Texture* addTexture, const std::string& name)
{
	textures.insert(std::make_pair(name.c_str(), addTexture));
	return addTexture;
}

GatesEngine::Texture* GatesEngine::TextureManager::GetTexture(const std::string& name)
{
	Texture* tex = textures[name.c_str()];
	if (!tex)tex = textures["nullTex"];
	return tex;
}
