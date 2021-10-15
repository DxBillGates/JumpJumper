#pragma once
#include <map>
#include <string>

namespace GatesEngine
{
	class GraphicsDevice;
	class Texture;
	class TextureManager
	{
	private:
		GraphicsDevice* graphicsDevice;
		std::map<std::string,Texture*> textures;
	public:
		TextureManager(GraphicsDevice* pGraphicsDevice);
		~TextureManager();
		Texture* Add(Texture* addTexture, const std::string& name);
		Texture* GetTexture(const std::string& name);
	};
}