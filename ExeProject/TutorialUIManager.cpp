#include "TutorialUIManager.h"


TutorialUIManager::TutorialUIManager(GatesEngine::GraphicsDevice* device)
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
	GatesEngine::Math::Vector3 centerOffset = { 1920,1080,0 };
	centerOffset /= 2;
	centerOffset.y -= 300;
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
				addUI.SetScaleWeight({ 100 });
				addUI.SetColor({ 1,0,0,1 });
				addUI.SetGraphicsDevice(device);
			}
		}

		// チュートリアルの説明をするUI情報
		{
			parent.uies.push_back(TutorialUI());
			auto& addUI = parent.uies.back();
			addUI.SetPosition({ centerOffset });
			addUI.SetScale({ 500,250,0 });
			addUI.SetScaleWeight({});
			addUI.SetColor({ 1,1,1,0.3f });
			addUI.SetGraphicsDevice(device);
		}
		// 背景となるUI情報
		{
			parent.uies.push_back(TutorialUI());
			auto& addUI = parent.uies.back();
			addUI.SetPosition({ centerOffset });
			addUI.SetScale({ 500,250,0 });
			addUI.SetScaleWeight({});
			addUI.SetColor({ 1,1,1,0.3f });
			addUI.SetGraphicsDevice(device);
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

void TutorialUIManager::Update(int currentClearCount, bool isAnimation)
{
	for (auto& parent : tutorialUI)
	{
		if (parent.state == currentState)
		{
			if (isAnimation)
			{
				int offset = clearUIoffset + currentClearCount - 1;
				if (offset < 0)offset = 0;
				parent.uies[offset].SetScaleAnimationFlag(true);
			}
			for (auto& ui : parent.uies)
			{
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
}
