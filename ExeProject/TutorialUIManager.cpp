#include "TutorialUIManager.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Manager/TextureManager.h"

TutorialUIManager::TutorialUIManager(GatesEngine::GraphicsDevice* device)
	: currentState(TutorialState::NORMAL_ATTACK)
{
	TutorialState tutorialStates[4] =
	{
		TutorialState::NORMAL_ATTACK,
		TutorialState::LOCKON_ATTACK,
		TutorialState::EMITTE_ATTACK,
		TutorialState::NORMAL_ENEMY
	};
	// 合計チュートリアル数分UIを生成して各ステートをセット
	tutorialUI.resize(4);
	int count = 0;
	float height = 1080, weight = 1920;
	float persent = 25;
	GatesEngine::Math::Vector3 centerOffset = { weight,height,0 };
	centerOffset /= 2;
	centerOffset.y -= (height / 100) * persent;
	centerOffset.x += (weight / 100) * persent * 1.25f;
	GatesEngine::Texture* textures[4];
	textures[0] = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("normalAttackTutorialTaskTex");
	textures[1] = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("lockonAttackTutorialTaskTex");
	textures[2] = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("emitteAttackTutorialTaskTex");
	textures[3] = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("moveTutorialTaskTex");
	for (auto& parent : tutorialUI)
	{
		parent.state = tutorialStates[count];
		clearUIoffset = 0;

		GatesEngine::Math::Vector3 offset[3];
		offset[0] = centerOffset - GatesEngine::Math::Vector3(125, 0, 0);
		offset[1] = centerOffset;
		offset[2] = centerOffset + GatesEngine::Math::Vector3(125, 0, 0);
		// クリア状況となるUI情報
		{
			for (int i = 0; i < 3; ++i)
			{
				parent.uies.push_back(TutorialUI());
				auto& addUI = parent.uies.back();
				addUI = parent.uies.back();
				addUI.SetPosition({ offset[i] });
				addUI.SetScale({ 50 / 2 });
				addUI.SetScaleWeight({ 75 });
				addUI.SetColor({ 1,0,0,1 });
				addUI.SetGraphicsDevice(device);
			}
		}

		// 背景となるUI情報
		{
			parent.uies.push_back(TutorialUI());
			auto& addUI = parent.uies.back();
			addUI.SetPosition({ centerOffset + GatesEngine::Math::Vector3(0,0,0) });
			addUI.SetScale({ 500,400,0 });
			addUI.SetScaleWeight({});
			addUI.SetColor({ 1,1,1,0.3f });
			addUI.SetGraphicsDevice(device);
		}
		// チュートリアルの説明をするUI情報
		{
			parent.uies.push_back(TutorialUI());
			auto& addUI = parent.uies.back();
			addUI.SetPosition({ centerOffset + GatesEngine::Math::Vector3(0,0,0) });
			addUI.SetScale({ 500,400,0 });
			addUI.SetScaleWeight({});
			addUI.SetColor({ 1,1,1,0.3f });
			addUI.SetGraphicsDevice(device);
			addUI.SetTexture(textures[count]);
		}
		++count;
	}
}

void TutorialUIManager::Initialize()
{
	for (auto& parent : tutorialUI)
	{
		for (auto& ui : parent.uies)
		{
			ui.Initialize();
		}
	}
}

void TutorialUIManager::Update(int currentClearCount, bool isAnimation, bool endScene)
{
	for (auto& parent : tutorialUI)
	{
		if (parent.state == currentState)
		{
			if (isAnimation)
			{
				int offset = clearUIoffset + currentClearCount - 1;
				if (offset < 0)offset = 0;
				if (!parent.uies[offset].GetStartFlag())
				{
					parent.uies[offset].SetScaleAnimationFlag(true);
				}
			}
			for (auto& ui : parent.uies)
			{
				if (endScene)
				{
					ui.SetEndFlag(true);
				}
				ui.Update();
			}
		}
	}
}

void TutorialUIManager::Draw()
{
	for (auto& parent : tutorialUI)
	{
		if (parent.state == currentState)
		{
			for (auto& ui : parent.uies)
			{
				ui.Draw();
			}
		}
	}
}

void TutorialUIManager::SetCurrentState(TutorialState currentState)
{
	this->currentState = currentState;
}

TutorialUI* TutorialUIManager::GetCurrentUIInfo(int currentClearCount)
{
	for (auto& parent : tutorialUI)
	{
		if (parent.state == currentState)
		{
			int offset = currentClearCount + clearUIoffset;
			return &parent.uies[offset];
		}
	}

	return nullptr;
}
