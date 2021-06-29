#include "Game.h"
#include "SampleScene.h"
#include "Header/Graphics/Graphics.h"
#include "PlayerBehaviour.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Component/Collider.h"
#include "PlayerSceneTransAnimation.h"

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
	sceneManager->AddScene(new SampleScene("SampleScene", this));
	sceneManager->ChangeScene("SampleScene");

	using namespace GatesEngine;
	using namespace GatesEngine::Math;

	auto* testTexShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Texture")), "Texture");
	testTexShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testLineShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Line")), "Line");
	testLineShader->Create({ InputLayout::POSITION,InputLayout::COLOR }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, true);

	auto* defaultMeshShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"DefaultMesh")), "DefaultMeshShader");
	defaultMeshShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV });

	auto* defaultSpriteShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"DefaultSprite")), "DefaultSpriteShader");
	defaultSpriteShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV });

	auto* testMultiRTVShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"TestMultiRTV")), "testMultiRTVShader");
	testMultiRTVShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false, 2);

	auto* testVGaussBlurShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"VGaussBlur")), "VGaussBlurShader");
	testVGaussBlurShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testHGaussBlurShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"HGaussBlur")), "HGaussBlurShader");
	testHGaussBlurShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testDOFShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"DepthOfField")), "DepthOfFieldShader");
	testDOFShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::CBV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testBrightnessSamplingShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"BrightnessSampling")), "BrightnessSamplingShader");
	testBrightnessSamplingShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV}, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* bloomShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Bloom")), "BloomShader");
	bloomShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* rgbShiftShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"RGBShift")), "RGBShiftShader");
	rgbShiftShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV}, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);
	//板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData;
	MeshCreater::CreateQuad({ 100,100 }, { 1,1 }, testMeshData);
	graphicsDevice.GetMeshManager()->Add("Plane")->Create(&graphicsDevice, testMeshData);

	//画面サイズ / 10 板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData2;
	MeshCreater::CreateQuad({ 1920,1080 }, { 1,1 }, testMeshData2);
	graphicsDevice.GetMeshManager()->Add("ScreenPlane")->Create(&graphicsDevice, testMeshData2);

	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData3;
	MeshCreater::Create2DQuad({ 192,108 }, { 1,1 }, testMeshData3);
	graphicsDevice.GetMeshManager()->Add("2DPlane")->Create(&graphicsDevice, testMeshData3);

	//グリッド生成
	MeshData<VertexInfo::Vertex_Color> testLineMeshData;
	MeshCreater::CreateGrid({ 10000,10000 }, 100, testLineMeshData);
	graphicsDevice.GetMeshManager()->Add("Grid")->Create(&graphicsDevice, testLineMeshData);

	//Cube生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData4;
	MeshCreater::CreateCube({ 50,50,50 }, testMeshData4);
	graphicsDevice.GetMeshManager()->Add("Cube")->Create(&graphicsDevice, testMeshData4);

	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData5;
	MeshCreater::CreateSphere({ 50,50,50 },16,16, testMeshData5);
	graphicsDevice.GetMeshManager()->Add("Sphere")->Create(&graphicsDevice, testMeshData5);

	MeshData<VertexInfo::Vertex_Color> testLineMeshData6;
	MeshCreater::CreateLineCube({55,55,55},Math::Vector4(0.5f,1,0,1),testLineMeshData6);
	graphicsDevice.GetMeshManager()->Add("LineCube")->Create(&graphicsDevice, testLineMeshData6);

	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(&graphicsDevice);
	g->AddBehavior<PlayerBehaviour>();
	g->GetComponent<PlayerBehaviour>()->SetCamera(&mainCamera);
	g->AddBehavior<PlayerSceneTransAnimation>();
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	g->GetCollider()->SetSize({ 50 });

	auto* e1 = gameObjectManager.Add(new GameObject());
	e1->SetGraphicsDevice(&graphicsDevice);
	e1->AddBehavior<NormalEnemyBehaviour>();
	e1->AddComponent<Collider>();
	e1->SetCollider();
	e1->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e1->GetCollider()->SetSize({ 50 });
	e1->SetName("enemy1");
	e1->SetTag("enemy");
	e1->GetTransform()->position = {-300,0,300};

	auto* e2 = gameObjectManager.Add(new GameObject());
	e2->SetGraphicsDevice(&graphicsDevice);
	e2->AddBehavior<NormalEnemyBehaviour>();
	e2->AddComponent<Collider>();
	e2->SetCollider();
	e2->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e2->GetCollider()->SetSize({ 50 });
	e2->SetName("enemy2");
	e2->SetTag("enemy");
	e2->GetTransform()->position = {300,0,300};

	auto* e3 = gameObjectManager.Add(new GameObject());
	e3->SetGraphicsDevice(&graphicsDevice);
	e3->AddBehavior<NormalEnemyBehaviour>();
	e3->AddComponent<Collider>();
	e3->SetCollider();
	e3->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e3->GetCollider()->SetSize({ 50 });
	e3->SetName("enemy3");
	e3->SetTag("enemy");
	e3->GetTransform()->position = {300,0,-300};

	auto* e4 = gameObjectManager.Add(new GameObject());
	e4->SetGraphicsDevice(&graphicsDevice);
	e4->AddBehavior<NormalEnemyBehaviour>();
	e4->AddComponent<Collider>();
	e4->SetCollider();
	e4->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e4->GetCollider()->SetSize({ 50 });
	e4->SetName("enemy4");
	e4->SetTag("enemy");
	e4->GetTransform()->position = {-300,0,-300};

	return true;
}

bool Game::Initialize()
{
	gameObjectManager.Start();
	timer.SetFrameRate(144);
	timer.SetIsShow(false);
	return true;
}

bool Game::Update()
{
	gameObjectManager.Update();
	sceneManager->Update();
	return true;
}

void Game::Draw()
{
	graphicsDevice.GetCBufferAllocater()->ResetCurrentUseNumber();
	graphicsDevice.GetCBVSRVUAVHeap()->Set();

	graphicsDevice.ClearRenderTarget({ 135,206,235,0 }, true);
	graphicsDevice.GetShaderManager()->GetShader("DefaultMeshShader")->Set();

	graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, mainCamera.GetData());
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });

	gameObjectManager.Draw();
	//Test用のGridや2D描画
	sceneManager->Draw();

	graphicsDevice.ScreenFlip();
}

