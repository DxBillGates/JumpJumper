#include "SelectScene.h"
#include "Header/Application/Application.h"
#include "Header/Graphics/Graphics.h"

SelectScene::SelectScene() : Scene("SelectScene", nullptr)
{
}

SelectScene::SelectScene(const char* sceneName) : Scene(sceneName, nullptr)
{
}

SelectScene::SelectScene(const char* sceneName, GatesEngine::Application* app) : Scene(sceneName, app)
{
	mainCamera = app->GetMainCamera();
}

SelectScene::~SelectScene()
{
}

void SelectScene::Initialize()
{
	sceneTranslater.SetTranslateState(SceneTranslater::TranslateState::DOWN);
	sceneTranslater.StartSceneTranslate(1);
	mainCamera->SetPosition({ 50,15,-200 });
	mainCamera->SetYawPitch({ GatesEngine::Math::ConvertToRadian(-60),0 });
	selectState = SelectState::NEXT_STAGE;
	input = GatesEngine::Input::GetInstance();
	waveValue = 0;
	waveFlag = true;
}

void SelectScene::Update()
{
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::A))
	{
		if (selectState == SelectState::TITLE)selectState = SelectState::NEXT_STAGE;
	}
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D))
	{
		if (selectState == SelectState::NEXT_STAGE)selectState = SelectState::TITLE;
	}
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::SPACE))
	{
		sceneTranslater.SetTranslateState(SceneTranslater::TranslateState::UP);
		sceneTranslater.StartSceneTranslate(1);
	}
	SceneTranslater::TranslateState oldSceneTranslaterState = sceneTranslater.GetTranslateState();
	sceneTranslater.Update(app->GetTimer()->GetElapsedTime());
	if (oldSceneTranslaterState == SceneTranslater::TranslateState::UP && oldSceneTranslaterState != sceneTranslater.GetTranslateState())
	{
		if (selectState == SelectState::NEXT_STAGE)
		{
			app->GetSceneManager()->ChangeScene("SampleScene");
		}
		else
		{
			app->GetSceneManager()->ChangeScene("TitleScene");
		}
	}


	if (waveValue < 0)
	{
		waveFlag = !waveFlag;
	}
	if (waveValue >= waveMax)
	{
		waveFlag = !waveFlag;
	}
	float elapsedTime = app->GetTimer()->GetElapsedTime();
	waveValue += (waveFlag) ? elapsedTime : -elapsedTime;
}

void SelectScene::Draw()
{
	static float angle = 0;
	angle -= 0.16f / 3;
	GatesEngine::Math::Matrix4x4 matrix = GatesEngine::Math::Matrix4x4::Scale(1);
	matrix *= GatesEngine::Math::Matrix4x4::RotationX(angle);
	matrix *= GatesEngine::Math::Matrix4x4::Translate({});

	graphicsDevice->GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, matrix);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, app->GetMainCamera()->GetData());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();

	sceneTranslater.Draw(graphicsDevice);

	if (waveValue > waveMax)waveValue = waveMax;
	float easingWave = GatesEngine::Math::Easing::EaseInCirc(waveValue);
	GatesEngine::Math::Vector3 nextStageUIScale = (selectState == SelectState::NEXT_STAGE) ? GatesEngine::Math::Vector3(1) + easingWave : GatesEngine::Math::Vector3(1);
	graphicsDevice->GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale(nextStageUIScale) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2 - 500,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	graphicsDevice->GetMeshManager()->GetMesh("2DPlane")->Draw();

	GatesEngine::Math::Vector3 goTitleUIScale = (selectState == SelectState::TITLE) ? GatesEngine::Math::Vector3(1) + easingWave : GatesEngine::Math::Vector3(1);
	graphicsDevice->GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale(goTitleUIScale) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(0, 0, 1, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	graphicsDevice->GetMeshManager()->GetMesh("2DPlane")->Draw();

}
