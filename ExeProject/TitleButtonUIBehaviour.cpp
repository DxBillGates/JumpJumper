#include "TitleButtonUIBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

TitleButtonUIBehaviour::TitleButtonUIBehaviour()
	: texture(nullptr)
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

	if (texture)
	{
		GatesEngine::ResourceManager::GetShaderManager()->GetShader("TextureSpriteShader")->Set();
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 1,1, alpha));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
		texture->Set(3);
		GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();
	}
	else
	{
		shaderManager->GetShader("DefaultSpriteShader")->Set();

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, alpha));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
		meshManager->GetMesh("2DPlane")->Draw();
	}
}

void TitleButtonUIBehaviour::SetTexture(GatesEngine::Texture* setTexture)
{
	texture = setTexture;
}
