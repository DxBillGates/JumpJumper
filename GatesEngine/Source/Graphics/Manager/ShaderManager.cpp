#include "..\..\..\Header\Graphics\Manager\ShaderManager.h"
#include "..\..\..\Header\Graphics\Shader.h"

GatesEngine::ShaderManager::ShaderManager(GraphicsDevice* pGraphicsDevice)
	: graphicsDevice(pGraphicsDevice)
{
}

GatesEngine::ShaderManager::~ShaderManager()
{
	for (auto itr = shaders.begin(); itr != shaders.end(); ++itr)
	{
		delete (*itr).second;
	}
}

GatesEngine::Shader* GatesEngine::ShaderManager::Add(Shader* addShader, const std::string& name)
{
	shaders.insert(std::make_pair(name.c_str(), addShader));
	return addShader;
}

GatesEngine::Shader* GatesEngine::ShaderManager::GetShader(const std::string& name)
{
	Shader* shader = shaders.at(name.c_str());
	return shader;
}
