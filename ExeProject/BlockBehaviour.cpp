#include "BlockBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"
#include "Header/Component/Collider.h"
#include "Header/Graphics/Manager/ResourceManager.h"

BlockBehaviour::BlockBehaviour()
	: hp(10)
	, mesh(nullptr)
	, shader(nullptr)
{
}

BlockBehaviour::~BlockBehaviour()
{
}

void BlockBehaviour::Start()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	mesh = GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube");
	shader = GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader");
}

void BlockBehaviour::Update()
{
}

void BlockBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	shader->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{0,0,0,1} });
	mesh->Draw();
}

void BlockBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}
