#include "Game.h"
#include "SampleScene.h"
#include "TitleScene.h"
#include "Stage1Scene.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Camera3D.h"
#include "Header/Graphics/DebugCamera.h"

Game::Game() :Application()
{
}

Game::Game(const GatesEngine::Math::Vector2& wSize, const char* title) : Application(wSize, title)
{
}

Game::~Game()
{
}

bool Game::LoadContents()
{
	using namespace GatesEngine;
	using namespace GatesEngine::Math;

	TextureManager* textureManager = ResourceManager::GetTextureManager();
	ShaderManager* shaderManager = ResourceManager::GetShaderManager();
	MeshManager* meshManager = ResourceManager::GetMeshManager();

	//シェーダーのロード＆設定
	auto* testTexShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"Texture")), "Texture");
	testTexShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true);

	auto* testLineShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"Line")), "Line");
	//testLineShader->SetInputLayout({ InputLayout::POSITION,InputLayout::COLOR });
	//testLineShader->SetRootParamerters({ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV });
	//testLineShader->SetPrimitiveTopology(GatesEngine::PrimiriveTopologyType::LINE);
	//testLineShader->Create();
	testLineShader->Create({ InputLayout::POSITION,InputLayout::COLOR }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, true);

	auto* defaultMeshShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"DefaultMesh")), "DefaultMeshShader");
	defaultMeshShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV });

	auto* defaultSpriteShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"DefaultSprite")), "DefaultSpriteShader");
	defaultSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV });

	auto* testMultiRTVShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"TestMultiRTV")), "testMultiRTVShader");
	testMultiRTVShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, true, 2);

	auto* testVGaussBlurShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"VGaussBlur")), "VGaussBlurShader");
	testVGaussBlurShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testHGaussBlurShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"HGaussBlur")), "HGaussBlurShader");
	testHGaussBlurShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testDOFShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"DepthOfField")), "DepthOfFieldShader");
	testDOFShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::CBV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testBrightnessSamplingShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"BrightnessSampling")), "BrightnessSamplingShader");
	testBrightnessSamplingShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* bloomShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"Bloom")), "BloomShader");
	bloomShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* rgbShiftShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"RGBShift")), "RGBShiftShader");
	rgbShiftShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* meshShadowShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"MeshShadow")), "MeshShadowShader");
	meshShadowShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });

	auto* pointShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"Point")), "PointShader");
	pointShader->Create({ InputLayout::POSITION }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ADD, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, true);

	auto* outlineShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"PostEffect_Outline")), "PostEffect_OutlineShader");
	outlineShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });

	auto* createParlinNoiseShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"CreateParlinNoiseTexture")), "CreateParlinNoiseTextureShader");
	createParlinNoiseShader->SetInputLayout({ InputLayout::POSITION,InputLayout::TEXCOORD });
	createParlinNoiseShader->SetRootParamerters({ RangeType::CBV,RangeType::CBV ,RangeType::CBV,RangeType::CBV });
	createParlinNoiseShader->SetIsUseDepth(false);
	createParlinNoiseShader->SetRtvCount(2);
	createParlinNoiseShader->CreatePipeline();

	auto* testTesselationShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"TestTesselation")), "TestTesselationShader");
	testTesselationShader->SetInputLayout({ InputLayout::POSITION,InputLayout::TEXCOORD,InputLayout::NORMAL });
	testTesselationShader->SetIsUseDepth(true);
	testTesselationShader->SetPrimitiveTopology(PrimiriveTopologyType::PATCH);
	testTesselationShader->SetRootParamerters({ RangeType::CBV,RangeType::CBV ,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV });
	testTesselationShader->CreatePipeline();

	auto* sceneTransFadeShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"SceneTransitionFade")), "SceneTransitionFadeShader");
	sceneTransFadeShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });

	auto* textureSpriteShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"TextureSprite")), "TextureSpriteShader");
	textureSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV });


	//板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData;
	MeshCreater::CreateQuad({ 1,1 }, { 1,1 }, testMeshData);
	meshManager->Add("Plane")->Create(&graphicsDevice, testMeshData);

	MeshData<VertexInfo::Vertex_UV_Normal> testMeshDataa;
	MeshCreater::CreateQuad({ 100,100 }, { 1,1 }, 10, testMeshDataa);
	meshManager->Add("DividePlane")->Create(&graphicsDevice, testMeshDataa);

	//画面サイズ板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData2;
	MeshCreater::CreateQuad({ 1920,1080 }, { 1,1 }, testMeshData2);
	meshManager->Add("ScreenPlane")->Create(&graphicsDevice, testMeshData2);

	//画面サイズ / 10 板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData3;
	MeshCreater::Create2DQuad({ 1,1 }, { 1,1 }, testMeshData3);
	meshManager->Add("2DPlane")->Create(&graphicsDevice, testMeshData3);

	//グリッド生成
	MeshData<VertexInfo::Vertex_Color> testLineMeshData;
	MeshCreater::CreateGrid({ 10000,10000 }, 100, testLineMeshData);
	meshManager->Add("Grid")->Create(&graphicsDevice, testLineMeshData);

	//Cube生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData4;
	MeshCreater::CreateCube({ 1,1,1 }, testMeshData4);
	meshManager->Add("Cube")->Create(&graphicsDevice, testMeshData4);

	//プログラムで球モデル生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData5;
	MeshCreater::CreateSphere({ 50,50,50 }, 16, 16, testMeshData5);
	meshManager->Add("Sphere")->Create(&graphicsDevice, testMeshData5);

	MeshData<VertexInfo::Vertex_Color> lineMeshData;
	MeshCreater::CreateLine({}, {1,1,1,1}, lineMeshData);
	meshManager->Add("Line")->Create(&graphicsDevice, lineMeshData);

	//BOXコライダー用のLineCube生成
	MeshData<VertexInfo::Vertex_Color> testLineMeshData6;
	MeshCreater::CreateLineCube({ 1,1,1 }, Math::Vector4(0, 0, 0, 1), testLineMeshData6);
	meshManager->Add("LineCube")->Create(&graphicsDevice, testLineMeshData6);

	//SPHEREコライダー用のLineCircle生成
	MeshData<VertexInfo::Vertex_Color> testLineMeshData7;
	MeshCreater::CreateLineCircle({ 1,1,1 }, 24, Math::Vector4(0, 0, 0, 1), testLineMeshData7);
	meshManager->Add("LineCircle")->Create(&graphicsDevice, testLineMeshData7);

	//点生成
	MeshData<VertexInfo::Vertex> testMeshData6;
	testMeshData6.GetVertices()->push_back({});
	testMeshData6.GetIndices()->push_back(0);
	meshManager->Add("Point")->Create(&graphicsDevice, testMeshData6);

	//モデル読み込み
	MeshData<VertexInfo::Vertex_UV_Normal> testModel;
	MeshCreater::LoadModelData("uv_sphere", testModel);
	meshManager->Add("testModel")->Create(&graphicsDevice, testModel);

	////モデル読み込み
	//MeshData<VertexInfo::Vertex_UV_Normal> skydomeModel;
	//MeshCreater::LoadModelData("skydome", skydomeModel);
	//meshManager->Add("skydome")->Create(&graphicsDevice, skydomeModel);

	////モデル読み込み
	//MeshData<VertexInfo::Vertex_UV_Normal> testG3M;
	//MeshCreater::LoadGates3DModelData("testModel", testG3M);
	//meshManager->Add("testG3M")->Create(&graphicsDevice, testG3M);

	auto* tex = textureManager->Add(new Texture(), "particleTex");
	tex->Load("particle.png", &graphicsDevice);

	sceneManager->AddScene(new SampleScene("SampleScene", this));
	sceneManager->AddScene(new TitleScene("TitleScene", this));
	sceneManager->AddScene(new Stage1Scene("Stage1Scene", this));
	sceneManager->ChangeSceneWithoutInitialize("Stage1Scene");
	//sceneManager->ChangeSceneWithoutInitialize("TitleScene");
	//sceneManager->ChangeScene("SampleScene");

	return true;
}

bool Game::Initialize()
{
	gameObjectManager.Start();
	timer.SetFrameRate(144);
	timer.SetIsShow(false);

	mainCamera = &camera;

	GatesEngine::GameObject* playerObject = sceneManager->GetCurrentScene()->GetGameObjectManager()->Find("player");
	if (playerObject)mainCamera = playerObject->GetComponent<PlayerBehaviour>()->GetSetCamera();
	else mainCamera = &camera;

	mainCamera->SetGraphicsDevice(&graphicsDevice);
	mainCamera->SetMainWindow(&mainWindow);
	graphicsDevice.SetMainCamera(mainCamera);
	sceneManager->GetCurrentScene()->Initialize();

	return true;
}

bool Game::Update()
{
	camera.Update();
	gameObjectManager.Update();
	sceneManager->Update();
	if(sceneManager->GetCurrentScene()->GetIsTerminateApplication())return false;
	return true;
}

bool Game::Draw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	graphicsDevice.GetCBufferAllocater()->ResetCurrentUseNumber();
	graphicsDevice.GetCBVSRVUAVHeap()->Set();

	sceneManager->Draw();
	sceneManager->LateDraw();

	if (!graphicsDevice.ScreenFlip())return false;

	return true;
}