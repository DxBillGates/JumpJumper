#include "Game.h"
#include "TitleScene.h"
#include "SampleScene.h"
#include "SelectScene.h"
#include "Stage1Scene.h"
#include "Stage2Scene.h"
#include "Header/Graphics/Graphics.h"

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

	auto* testTexShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Texture")), "Texture");
	testTexShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* testLineShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Line")), "Line");
	//testLineShader->SetInputLayout({ InputLayout::POSITION,InputLayout::COLOR });
	//testLineShader->SetRootParamerters({ RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV });
	//testLineShader->SetPrimitiveTopology(GatesEngine::PrimiriveTopologyType::LINE);
	//testLineShader->Create();
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
	testBrightnessSamplingShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* bloomShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Bloom")), "BloomShader");
	bloomShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* rgbShiftShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"RGBShift")), "RGBShiftShader");
	rgbShiftShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* meshShadowShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"MeshShadow")), "MeshShadowShader");
	meshShadowShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });

	auto* pointShader = graphicsDevice.GetShaderManager()->Add(new Shader(&graphicsDevice, std::wstring(L"Point")),"PointShader");
	pointShader->Create({ InputLayout::POSITION }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, true);

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
	MeshCreater::CreateCube({ 1,1,1 }, testMeshData4);
	graphicsDevice.GetMeshManager()->Add("Cube")->Create(&graphicsDevice, testMeshData4);

	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData5;
	MeshCreater::CreateSphere({ 50,50,50 }, 16, 16, testMeshData5);
	graphicsDevice.GetMeshManager()->Add("Sphere")->Create(&graphicsDevice, testMeshData5);

	MeshData<VertexInfo::Vertex_Color> testLineMeshData6;
	MeshCreater::CreateLineCube({ 1,1,1 }, Math::Vector4(0, 0, 0, 1), testLineMeshData6);
	graphicsDevice.GetMeshManager()->Add("LineCube")->Create(&graphicsDevice, testLineMeshData6);

	MeshData<VertexInfo::Vertex_Color> testLineMeshData7;
	MeshCreater::CreateLineCircle({ 1,1,1 },24, Math::Vector4(0, 0, 0, 1), testLineMeshData7);
	graphicsDevice.GetMeshManager()->Add("LineCircle")->Create(&graphicsDevice, testLineMeshData7);

	MeshData<VertexInfo::Vertex> testMeshData6;
	testMeshData6.GetVertices()->push_back({});
	testMeshData6.GetIndices()->push_back(0);
	graphicsDevice.GetMeshManager()->Add("Point")->Create(&graphicsDevice, testMeshData6);

	//MeshData<VertexInfo::Vertex_UV_Normal> testModel;
	//MeshCreater::LoadModelData("testModel", testModel);
	//graphicsDevice.GetMeshManager()->Add("testModel")->Create(&graphicsDevice, testModel);

	shadowRenderTex.Create(&graphicsDevice, { 1920,1080 });
	shadowDepthTex.Create(&graphicsDevice, { 1920,1080 });

	sceneManager->AddScene(new TitleScene("TitleScene", this));
	sceneManager->AddScene(new SampleScene("SampleScene", this));
	sceneManager->AddScene(new SelectScene("SelectScene", this));
	sceneManager->AddScene(new Stage1Scene("Stage1Scene", this));
	sceneManager->AddScene(new Stage2Scene("Stage2Scene", this));
	sceneManager->ChangeScene("Stage1Scene");
	//sceneManager->ChangeScene("TitleScene");

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

bool Game::Draw()
{
	graphicsDevice.GetCBufferAllocater()->ResetCurrentUseNumber();
	graphicsDevice.GetCBVSRVUAVHeap()->Set();

	//影深度描画
	graphicsDevice.ClearRenderTarget({ 0,0,0,1 }, true, &shadowRenderTex, &shadowDepthTex);
	graphicsDevice.GetShaderManager()->GetShader("DefaultMeshShader")->Set();

	GatesEngine::GameObject* player = sceneManager->GetCurrentScene()->GetGameObjectManager()->Find("player");
	GatesEngine::Math::Vector3 pos = (player) ? player->GetTransform()->position : GatesEngine::Math::Vector3();
	GatesEngine::B2 lightViewData;
	float angle = 90;
	GatesEngine::Math::Vector3 dir = GatesEngine::Math::Vector3(0, 0, 1).Normalize() * GatesEngine::Math::Matrix4x4::RotationX(GatesEngine::Math::ConvertToRadian(angle));
	GatesEngine::Math::Vector3 up = GatesEngine::Math::Vector3(0, 1, 0).Normalize() * GatesEngine::Math::Matrix4x4::RotationX(GatesEngine::Math::ConvertToRadian(angle));
	lightViewData.viewMatrix = GatesEngine::Math::Matrix4x4::GetViewMatrixLookTo({ GatesEngine::Math::Vector3(0,10000,0) + pos},dir, up);
	lightViewData.projMatrix = GatesEngine::Math::Matrix4x4::GetOrthographMatrix({1000,1000},1,20000);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, lightViewData);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,-1,0,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });

	gameObjectManager.Draw();
	//Test用のGridや2D描画
	sceneManager->Draw();


	graphicsDevice.ClearRenderTarget({ 0,0,0,0 }, true);

	graphicsDevice.GetShaderManager()->GetShader("MeshShadowShader")->Set();

	graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, mainCamera.GetData());
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });
	GatesEngine::Math::Matrix4x4 lightViewMatrix = lightViewData.viewMatrix * lightViewData.projMatrix;
 	graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	shadowDepthTex.Set(5);

	gameObjectManager.Draw();
	//Test用のGridや2D描画
	sceneManager->Draw();

	graphicsDevice.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice.GetShaderManager()->GetShader("Line")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, app->GetMainCamera()->GetData());

	for (int i = 0; i < 1; ++i)
	{
		graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,-20000.0f * i,0 }));
		graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
		graphicsDevice.GetMeshManager()->GetMesh("Grid")->Draw();
	}

	gameObjectManager.LateDraw();
	sceneManager->LateDraw();

	if (!graphicsDevice.ScreenFlip())return false;

	return true;
}

