#include "..\..\Header\Component\Collider.h"
#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Graphics\Graphics.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"
#include "..\..\Header\Graphics\CBufferStruct.h"

GatesEngine::Collider::Collider()
	: type(ColliderType::CUBE)
	, size({ 1 })
	, color({ 0,1,0,0 })
	, transform({})
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
	//transform = (*gameObject->GetTransform());
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

	GatesEngine::Math::Matrix4x4 matrix = Math::Matrix4x4::Identity();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	shader->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, graphicsDevice->GetMainCamera()->GetData());


	Math::Vector3 s = gameObject->GetTransform()->scale;
	Math::Vector3 p = gameObject->GetTransform()->position;
	if (type == ColliderType::CUBE)
	{
		matrix = Math::Matrix4x4::Scale({ s.x * transform.scale.x,s.y * transform.scale.y,s.z * transform.scale.z }) * Math::Matrix4x4::Translate(p + transform.position);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, matrix);
	}
	else
	{
		matrix = Math::Matrix4x4::Scale({ s.x * transform.scale.x,s.y * transform.scale.y,s.z * transform.scale.z }) * Math::Matrix4x4::Translate(p + transform.position);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, matrix * transform.GetMatrix());
		mesh->Draw();

		matrix = Math::Matrix4x4::Scale({ s.x * transform.scale.x,s.y * transform.scale.y,s.z * transform.scale.z }) * Math::Matrix4x4::RotationX(Math::ConvertToRadian(90)) * Math::Matrix4x4::Translate(p + transform.position);

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, matrix * transform.GetMatrix());
		mesh->Draw();


		matrix = Math::Matrix4x4::Scale({ s.x * transform.scale.x,s.y * transform.scale.y,s.z * transform.scale.z }) * Math::Matrix4x4::RotationZ(Math::ConvertToRadian(90)) * Math::Matrix4x4::Translate(p + transform.position);

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, matrix * transform.GetMatrix());
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

void GatesEngine::Collider::SetPosition(const Math::Vector3& pos)
{
	transform.position = pos;
}

void GatesEngine::Collider::SetTransform(const GatesEngine::Transform& transform)
{
	this->transform = transform;
}

GatesEngine::ColliderType GatesEngine::Collider::GetType()
{
	return type;
}

void GatesEngine::Collider::SetSize(const Math::Vector3& sSize)
{
	transform.scale = sSize;
}

GatesEngine::Math::Vector3 GatesEngine::Collider::GetSize()
{
	return transform.scale;
}

void GatesEngine::Collider::SetCollisionTreeObject(CollisionTreeObject* obj)
{
	treeObj = obj;
}

GatesEngine::CollisionTreeObject* GatesEngine::Collider::GetCollisionTreeObject()
{
	return treeObj;
}

GatesEngine::Transform* GatesEngine::Collider::GetTransform()
{
	return &transform;
}
