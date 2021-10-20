#include "..\..\Header\Component\Collider.h"
#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Graphics\Graphics.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"
#include "..\..\Header\Graphics\CBufferStruct.h"

GatesEngine::Collider::Collider()
	: type(ColliderType::CUBE)
	, size({ 1 })
	, color({ 0,1,0,0 })
	, treeObj(nullptr)
	, mesh(nullptr)
	, shader(nullptr)
	, isOldHit(false)
	, hitFrameCount(0)
{
}

GatesEngine::Collider::~Collider()
{
	delete treeObj;
}

void GatesEngine::Collider::Start()
{
	GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	mesh = (type == ColliderType::CUBE) ? graphicsDevice->GetMeshManager()->GetMesh("LineCube") : graphicsDevice->GetMeshManager()->GetMesh("LineCircle");
	shader = graphicsDevice->GetShaderManager()->GetShader("Line");
}

void GatesEngine::Collider::Update()
{
	if (isOldHit)
	{
		const int MAX_FRAME = 10;
		if (hitFrameCount >= MAX_FRAME)
		{
			isOldHit = false;
			color = { 0.5f,1,0,0 };
			hitFrameCount = 0;
		}
		++hitFrameCount;
	}
}

void GatesEngine::Collider::OnDraw()
{
}

void GatesEngine::Collider::OnLateDraw()
{
#ifdef _DEBUG
	float addScale = 1.015f;
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	GatesEngine::Math::Matrix4x4 lineCubeMatrix = GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
	lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	shader->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, graphicsDevice->GetMainCamera()->GetData());

	if (type == ColliderType::CUBE)
	{
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
	}
	else
	{
		lineCubeMatrix = GatesEngine::Math::Matrix4x4::Identity();
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
		mesh->Draw();

		lineCubeMatrix = GatesEngine::Math::Matrix4x4::Identity();
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::RotationX(Math::ConvertToRadian(90));
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
		mesh->Draw();


		lineCubeMatrix = GatesEngine::Math::Matrix4x4::Identity();
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::RotationZ(Math::ConvertToRadian(90));
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
	}
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),color });
	mesh->Draw();
#endif
}

void GatesEngine::Collider::SetColor(const Math::Vector4& c)
{
	isOldHit = true;
	color = c;
}

void GatesEngine::Collider::SetType(ColliderType sType)
{
	type = sType;
}

GatesEngine::ColliderType GatesEngine::Collider::GetType()
{
	return type;
}

void GatesEngine::Collider::SetSize(const Math::Vector3& sSize)
{
	size = sSize;
}

GatesEngine::Math::Vector3 GatesEngine::Collider::GetSize()
{
	return size;
}

void GatesEngine::Collider::SetCollisionTreeObject(CollisionTreeObject* obj)
{
	treeObj = obj;
}

GatesEngine::CollisionTreeObject* GatesEngine::Collider::GetCollisionTreeObject()
{
	return treeObj;
}
