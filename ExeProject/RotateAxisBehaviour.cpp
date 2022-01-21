#include "RotateAxisBehaviour.h"
#include "Header/Math/Transform.h"
#include "Header/Math/Quaternion.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Util/Random.h"

RotateAxisBehaviour::RotateAxisBehaviour()
	: axis({ 0,1,0 })
	, center({ 0 })
	, angle(0)
	, rotatePower(1)
{
}

RotateAxisBehaviour::~RotateAxisBehaviour()
{
}

void RotateAxisBehaviour::Start()
{
	GatesEngine::Math::Vector3 pos;
	float range = 32767;
	pos.x = GatesEngine::Random::Rand(-range, range);
	pos.y = GatesEngine::Random::Rand(-range, range);
	pos.z = GatesEngine::Random::Rand(-range, range);
	gameObject->GetTransform()->position = { pos / 5 };
	originPosition = gameObject->GetTransform()->position;
	angle = 0;
	isFaceCenter = true;
	addValue = GatesEngine::Random::Rand(1, 2);
	isRotateRight = GatesEngine::Random::Rand(-10, 10) >= 0 ? true : false;
}

void RotateAxisBehaviour::Update()
{
	// –ˆƒtƒŒ[ƒ€‚Ì‰ñ“]’l
	const float INCREASE_ANGLE_VALUE = 0.016f;

	GatesEngine::Transform* transform = gameObject->GetTransform();
	GatesEngine::Math::Vector3 pos = transform->position;
	GatesEngine::Math::Vector3 dirCenter = center - pos;
	float dirCenterLength = dirCenter.Length();
	float dirVectorValue = dirCenterLength / 100;
	dirCenter = dirCenter.Normalize();

	if (!isFaceCenter)dirCenter = -dirCenter;
	if (!isSetCenter)dirVectorValue = 0;

	GatesEngine::Math::Quaternion posQuaternion = { pos.x,pos.y,pos.z,0 };
	GatesEngine::Math::Quaternion quaternion = { axis,angle };
	GatesEngine::Math::Quaternion conjugateQuaternion = GatesEngine::Math::Quaternion::Conjugate(quaternion);

	quaternion = quaternion * posQuaternion * conjugateQuaternion;
	GatesEngine::Math::Vector3 newPos = { quaternion.x,quaternion.y,quaternion.z };
	transform->position = newPos + dirCenter * dirVectorValue + offset;

	float rotateValue = INCREASE_ANGLE_VALUE * addValue;
	angle = isRotateRight ? rotateValue : -rotateValue;
}

void RotateAxisBehaviour::SetAxis(const GatesEngine::Math::Vector3& axis)
{
	this->axis = axis;
}

void RotateAxisBehaviour::SetCenter(const GatesEngine::Math::Vector3& pos)
{
	isSetCenter = true;
	center = pos;
}

void RotateAxisBehaviour::SetOffset(const GatesEngine::Math::Vector3& pos)
{
	offset = pos;
}

void RotateAxisBehaviour::SetRotatePower(float value)
{
	rotatePower = value;
}

void RotateAxisBehaviour::SetIsFaceCenter(bool flag)
{
	isFaceCenter = flag;
}

GatesEngine::Math::Vector3 RotateAxisBehaviour::GetOriginPosition()
{
	return originPosition;
}

void RotateAxisBehaviour::SetAddValue(float value)
{
	addValue = value;
}
