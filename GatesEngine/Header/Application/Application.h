#pragma once
#include "..\..\Header\Math\Vector2.h"
#include "..\..\Header\Util\Timer.h"
#include "..\Window\Window.h"
#include "..\Input\Input.h"
#include "..\..\Header\Scene\SceneManager.h"
#include "..\..\Header\GameObject\GameObjectManager.h"
#include "..\Graphics\GraphicsDevice.h"
#include "..\Graphics\CBuffer.h"
#include "..\Graphics\Camera.h"

namespace GatesEngine
{
	class Application
	{
	private:
	protected:
		Util::Timer timer;
		Window mainWindow;
		Input* input;
		SceneManager* sceneManager;
		GameObjectManager gameObjectManager;
		GraphicsDevice graphicsDevice;
		CBuffer<B3> worldLightInfo;

		Camera mainCamera;
	public:
		Application();
		Application(const Math::Vector2& wSize, const char* title);
		virtual ~Application();
		virtual bool LoadContents();
		virtual bool Initialize();
		virtual bool Update();
		virtual bool Draw();
		virtual void Run();

		Util::Timer* GetTimer();
		SceneManager* GetSceneManager();
		GraphicsDevice* GetGraphicsDevice();
		Window* GetWindow();
		Input* GetInput();
		Camera* GetMainCamera();
	};
}