#pragma once

namespace GatesEngine
{
	class GraphicsDevice;
	class TextureManager;
	class ShaderManager;
	class MeshManager;
	class ResourceManager
	{
	private:
		static TextureManager* textureManager;
		static ShaderManager* shaderManager;
		static MeshManager* meshManager;
		static bool isInitialize;
	public:
		static void Initialize(GraphicsDevice* gDevice);
		static void Terminate();
		static TextureManager* GetTextureManager();
		static ShaderManager* GetShaderManager();
		static MeshManager* GetMeshManager();
	};
}