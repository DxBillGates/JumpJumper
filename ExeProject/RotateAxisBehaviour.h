#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Vector3.h"

class RotateAxisBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::Math::Vector3 axis;
	GatesEngine::Math::Vector3 center;
	float angle;
	float rotatePower;
	bool isFaceCenter;
	GatesEngine::Math::Vector3 originPosition;
	float addValue;
public:
	RotateAxisBehaviour();
	~RotateAxisBehaviour();
	void Start() override;
	void Update() override;
	void SetAxis(const GatesEngine::Math::Vector3& axis);
	void SetCenter(const GatesEngine::Math::Vector3& pos);
	void SetRotatePower(float value);
	void SetIsFaceCenter(bool flag);
	GatesEngine::Math::Vector3 GetOriginPosition();
	void SetAddValue(float value);
};
