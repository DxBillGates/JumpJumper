#include "..\..\Header\Component\Collider.h"
#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Graphics\Graphics.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"

GatesEngine::Collider::Collider()
	: type(ColliderType::CUBE)
	, size({ 1 })
	, treeObj(nullptr)
{
}

GatesEngine::Collider::~Collider()
{
	delete treeObj;
}

void GatesEngine::Collider::OnDraw()
{
#ifdef _DEBUG
	float addScale = 1.025f;
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	GatesEngine::Math::Matrix4x4 lineCubeMatrix = GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
	lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice->GetShaderManager()->GetShader("Line")->Set();

	if (type == ColliderType::CUBE)
	{
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
		graphicsDevice->GetMeshManager()->GetMesh("LineCube")->Draw();
	}
	else
	{
		lineCubeMatrix = GatesEngine::Math::Matrix4x4::Identity();
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
		graphicsDevice->GetMeshManager()->GetMesh("LineCircle")->Draw();

		lineCubeMatrix = GatesEngine::Math::Matrix4x4::Identity();
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::RotationX(Math::ConvertToRadian(90));
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
		graphicsDevice->GetMeshManager()->GetMesh("LineCircle")->Draw();


		lineCubeMatrix = GatesEngine::Math::Matrix4x4::Identity();
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize() * addScale);
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::RotationZ(Math::ConvertToRadian(90));
		lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
		graphicsDevice->GetMeshManager()->GetMesh("LineCircle")->Draw();
	}
#endif
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
