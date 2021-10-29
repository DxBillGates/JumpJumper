#include "..\..\Header\Application\Application.h"
#include "..\..\Header\Graphics\Manager\ResourceManager.h"

GatesEngine::Application::Application()
	:Application({ 1920,1080 }, "GatesEngine")
{
}

GatesEngine::Application::Application(const Math::Vector2& wSize, const char* title)
	: timer(Util::Timer())
	, mainWindow(Window())
	, input(Input::GetInstance())
	, sceneManager(SceneManager::GetInstance())
	, gameObjectManager(GameObjectManager())
	, worldLightInfo({})
	, mainCamera(nullptr)
{
	mainWindow.Create(wSize, title);
	mainWindow.PreviewWindow();
	input->Create(mainWindow.GetHandle(), mainWindow.GetHInstance());
	graphicsDevice.Create(&mainWindow);

	worldLightInfo.Create(&graphicsDevice,3);
	worldLightInfo.Map({ Math::Vector4(0,1,1,0).Normalize(),Math::Vector4(1,0,0,1) });


	ResourceManager::Initialize(&graphicsDevice);
}

GatesEngine::Application::~Application()
{
	ResourceManager::Terminate();
}

bool GatesEngine::Application::LoadContents()
{
	return true;
}

bool GatesEngine::Application::Initialize()
{
	return true;
}
				
bool GatesEngine::Application::Update()
{
	return true;
}

bool GatesEngine::Application::Draw()
{
	return true;
}

void GatesEngine::Application::Run()
{
	input->Initialize();

	if (!LoadContents())return;
	if (!Initialize())return;


	while (!input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::ESC))
	{
		if (timer.Update())continue;
		input->Update();
		mainCamera->Update();
		if (!Update())break;
		if(!Draw())break;
		if (!mainWindow.ProcessMessage())break;
	}
}

GatesEngine::Util::Timer* GatesEngine::Application::GetTimer()
{
	return &timer;
}

GatesEngine::SceneManager* GatesEngine::Application::GetSceneManager()
{
	return sceneManager;
}

GatesEngine::GraphicsDevice* GatesEngine::Application::GetGraphicsDevice()
{
	return &graphicsDevice;
}

GatesEngine::Window* GatesEngine::Application::GetWindow()
{
	return &mainWindow;
}

GatesEngine::Input* GatesEngine::Application::GetInput()
{
	return input;
}

GatesEngine::Camera* GatesEngine::Application::GetMainCamera()
{
	return mainCamera;
}
