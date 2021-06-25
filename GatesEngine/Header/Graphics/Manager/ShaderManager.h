#pragma once
#include <map>
#include <string>

namespace GatesEngine
{
	class GraphicsDevice;
	class Shader;
	class ShaderManager
	{
	private:
		GraphicsDevice* graphicsDevice;
		std::map<std::string, Shader*> shaders;
	public:
		ShaderManager(GraphicsDevice* pGraphicsDevice);
		~ShaderManager();
		Shader* Add(Shader* addShader, const std::string& name);
		Shader* GetShader(const std::string& name);
	};
}
