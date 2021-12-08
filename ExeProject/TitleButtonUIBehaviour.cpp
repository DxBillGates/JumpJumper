#include "TitleButtonUIBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

TitleButtonUIBehaviour::TitleButtonUIBehaviour()
{
}

TitleButtonUIBehaviour::~TitleButtonUIBehaviour()
{
}

void TitleButtonUIBehaviour::Start()
{
}

void TitleButtonUIBehaviour::Update()
{
}

void TitleButtonUIBehaviour::OnLateDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	if (!graphicsDevice)return;

	shaderManager->GetShader("DefaultSpriteShader")->Set();

	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, alpha));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	meshManager->GetMesh("2DPlane")->Draw();
}
