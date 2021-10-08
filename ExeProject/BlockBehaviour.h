#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Graphics/Mesh.h"
#include "Header/Graphics/Shader.h"

class BlockBehaviour : public GatesEngine::Behaviour
{
private:
	float hp;
	GatesEngine::Mesh* mesh;
	GatesEngine::Shader* shader;
public:
	BlockBehaviour();
	~BlockBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
};

