#include "Game.h"
#include "SampleScene.h"
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
	testTexShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

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
	testBrightnessSamplingShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* bloomShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"Bloom")), "BloomShader");
	bloomShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* rgbShiftShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"RGBShift")), "RGBShiftShader");
	rgbShiftShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, false);

	auto* meshShadowShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"MeshShadow")), "MeshShadowShader");
	meshShadowShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD ,InputLayout::NORMAL }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV });

	auto* pointShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"Point")), "PointShader");
	pointShader->Create({ InputLayout::POSITION }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV }, BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT, true);

	auto* outlineShader = shaderManager->Add(new Shader(&graphicsDevice, std::wstring(L"PostEffect_Outline")), "PostEffect_OutlineShader");
	outlineShader->Create({ InputLayout::POSITION,InputLayout::TEXCOORD }, { RangeType::CBV,RangeType::CBV,RangeType::CBV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV,RangeType::SRV });

	//板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData;
	MeshCreater::CreateQuad({ 100,100 }, { 1,1 }, testMeshData);
	meshManager->Add("Plane")->Create(&graphicsDevice, testMeshData);

	//画面サイズ板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData2;
	MeshCreater::CreateQuad({ 1920,1080 }, { 1,1 }, testMeshData2);
	meshManager->Add("ScreenPlane")->Create(&graphicsDevice, testMeshData2);

	//画面サイズ / 10 板ポリ生成
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData3;
	MeshCreater::Create2DQuad({ 192,108 }, { 1,1 }, testMeshData3);
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

	shadowRenderTex.Create(&graphicsDevice, { 1920,1080 });
	shadowDepthTex.Create(&graphicsDevice, { 1920,1080 });
	resultRenderTex.Create(&graphicsDevice, { 1920,1080 }, GatesEngine::Math::Vector4(141, 219, 228, 255) / 255.0f);
	resultDepthTex.Create(&graphicsDevice, { 1920,1080 });
	lateDrawResultRenderTex.Create(&graphicsDevice, { 1920,1080 }, GatesEngine::Math::Vector4(141, 219, 228, 255) / 255.0f);
	lateDrawResultDepthTex.Create(&graphicsDevice, { 1920,1080 });
	resultRenderShadowTex.Create(&graphicsDevice, { 1920,1080 }, GatesEngine::Math::Vector4(141, 219, 228, 255) / 255.0f);

	sceneManager->AddScene(new SampleScene("SampleScene", this));
	sceneManager->AddScene(new Stage1Scene("Stage1Scene", this));
	sceneManager->ChangeScene("Stage1Scene");


	mainCamera = &camera;
	//mainCamera = sceneManager->GetCurrentScene()->GetGameObjectManager()->Find("player")->GetComponent<PlayerBehaviour>()->GetSetCamera();
	mainCamera->SetGraphicsDevice(&graphicsDevice);
	mainCamera->SetMainWindow(&mainWindow);
	graphicsDevice.SetMainCamera(mainCamera);

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
	//camera.Update();
	gameObjectManager.Update();
	sceneManager->Update();
	return true;
}

bool Game::Draw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	graphicsDevice.GetCBufferAllocater()->ResetCurrentUseNumber();
	graphicsDevice.GetCBVSRVUAVHeap()->Set();

	//シャドウマップ用深度描画
	graphicsDevice.ClearRenderTarget({ 0,0,0,1 }, true, &shadowRenderTex, &shadowDepthTex);
	shaderManager->GetShader("DefaultMeshShader")->Set();

	//プレイヤーの上空から一度描画
	GatesEngine::GameObject* player = sceneManager->GetCurrentScene()->GetGameObjectManager()->Find("player");
	GatesEngine::Math::Vector3 pos = (player) ? player->GetTransform()->position : GatesEngine::Math::Vector3();
	GatesEngine::B2 lightViewData;
	float angle = 90;
	GatesEngine::Math::Vector3 dir = GatesEngine::Math::Vector3(0, 0, 1).Normalize() * GatesEngine::Math::Matrix4x4::RotationX(GatesEngine::Math::ConvertToRadian(angle));
	GatesEngine::Math::Vector3 up = GatesEngine::Math::Vector3(0, 1, 0).Normalize() * GatesEngine::Math::Matrix4x4::RotationX(GatesEngine::Math::ConvertToRadian(angle));
	lightViewData.viewMatrix = GatesEngine::Math::Matrix4x4::GetViewMatrixLookTo({ GatesEngine::Math::Vector3(0,10000,0) + pos }, dir, up);
	lightViewData.projMatrix = GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 2000,2000 }, 1, 20000);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, lightViewData);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,-1,0,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });

	//シーンの描画
	sceneManager->Draw();


	////深度テクスチャを利用してプレイヤー視点で描画

	//graphicsDevice.ClearRenderTarget(GatesEngine::Math::Vector4(141,219,228,255)/255.0f, true, &resultRenderTex, &resultDepthTex);

	//graphicsDevice.GetShaderManager()->GetShader("MeshShadowShader")->Set();

	//graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, mainCamera.GetData());
	//graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });
	//GatesEngine::Math::Matrix4x4 lightViewMatrix = lightViewData.viewMatrix * lightViewData.projMatrix;
	//graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	//shadowDepthTex.Set(5);

	////シーンの描画
	//sceneManager->Draw();


	graphicsDevice.SetMultiRenderTarget({ &resultRenderTex,&resultRenderShadowTex }, &resultDepthTex, GatesEngine::Math::Vector4(141, 219, 228, 255) / 255.0f);
	//深度テクスチャを利用してプレイヤー視点で描画
	shaderManager->GetShader("testMultiRTVShader")->Set();

	//graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, mainCamera.GetData());
	mainCamera->Set(2);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });
	GatesEngine::Math::Matrix4x4 lightViewMatrix = lightViewData.viewMatrix * lightViewData.projMatrix;
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	shadowDepthTex.Set(5);

	//シーンの描画
	sceneManager->Draw();


	//グリッドの描画
	graphicsDevice.ClearRenderTarget(GatesEngine::Math::Vector4(141, 219, 228, 255) / 255.0f, true, &lateDrawResultRenderTex, &lateDrawResultDepthTex);
	graphicsDevice.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	shaderManager->GetShader("Line")->Set();
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,0,0 }));
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
	meshManager->GetMesh("Grid")->Draw();

	//スプライトやコライダーのワイヤーフレーム表示
	//gameObjectManager.LateDraw();
	sceneManager->LateDraw();

	//描画結果から深度テクスチャを利用してアウトライン付与してを描画
	graphicsDevice.ClearRenderTarget({ 0,0,0,1 }, true);

	shaderManager->GetShader("PostEffect_OutlineShader")->Set();
	graphicsDevice.GetCBVSRVUAVHeap()->Set();
	graphicsDevice.GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 10,10,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	static GatesEngine::Math::Vector4 color = { 0,0,0,1 };
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D1))color = { 1,0,0,1 };
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D2))color = { 0,1,0,1 };
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D3))color = { 0,0,1,1 };
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D4))color = { 0,0,0,1 };
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(1, color);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	resultDepthTex.Set(3);
	resultRenderTex.Set(4);
	lateDrawResultDepthTex.Set(5);
	lateDrawResultRenderTex.Set(6);
	resultRenderShadowTex.Set(7);
	meshManager->GetMesh("2DPlane")->Draw();

	if (!graphicsDevice.ScreenFlip())return false;

	return true;
}

