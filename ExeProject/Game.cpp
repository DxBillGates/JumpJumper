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

	//�V�F�[�_�[�̃��[�h���ݒ�
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


	//�|������
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData;
	MeshCreater::CreateQuad({ 100,100 }, { 1,1 }, testMeshData);
	meshManager->Add("Plane")->Create(&graphicsDevice, testMeshData);

	MeshData<VertexInfo::Vertex_UV_Normal> testMeshDataa;
	MeshCreater::CreateQuad({ 100,100 }, { 1,1 }, 10, testMeshDataa);
	meshManager->Add("DividePlane")->Create(&graphicsDevice, testMeshDataa);

	//��ʃT�C�Y�|������
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData2;
	MeshCreater::CreateQuad({ 1920,1080 }, { 1,1 }, testMeshData2);
	meshManager->Add("ScreenPlane")->Create(&graphicsDevice, testMeshData2);

	//��ʃT�C�Y / 10 �|������
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData3;
	MeshCreater::Create2DQuad({ 192,108 }, { 1,1 }, testMeshData3);
	meshManager->Add("2DPlane")->Create(&graphicsDevice, testMeshData3);

	//�O���b�h����
	MeshData<VertexInfo::Vertex_Color> testLineMeshData;
	MeshCreater::CreateGrid({ 10000,10000 }, 100, testLineMeshData);
	meshManager->Add("Grid")->Create(&graphicsDevice, testLineMeshData);

	//Cube����
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData4;
	MeshCreater::CreateCube({ 1,1,1 }, testMeshData4);
	meshManager->Add("Cube")->Create(&graphicsDevice, testMeshData4);

	//�v���O�����ŋ����f������
	MeshData<VertexInfo::Vertex_UV_Normal> testMeshData5;
	MeshCreater::CreateSphere({ 50,50,50 }, 16, 16, testMeshData5);
	meshManager->Add("Sphere")->Create(&graphicsDevice, testMeshData5);

	//BOX�R���C�_�[�p��LineCube����
	MeshData<VertexInfo::Vertex_Color> testLineMeshData6;
	MeshCreater::CreateLineCube({ 1,1,1 }, Math::Vector4(0, 0, 0, 1), testLineMeshData6);
	meshManager->Add("LineCube")->Create(&graphicsDevice, testLineMeshData6);

	//SPHERE�R���C�_�[�p��LineCircle����
	MeshData<VertexInfo::Vertex_Color> testLineMeshData7;
	MeshCreater::CreateLineCircle({ 1,1,1 }, 24, Math::Vector4(0, 0, 0, 1), testLineMeshData7);
	meshManager->Add("LineCircle")->Create(&graphicsDevice, testLineMeshData7);

	//�_����
	MeshData<VertexInfo::Vertex> testMeshData6;
	testMeshData6.GetVertices()->push_back({});
	testMeshData6.GetIndices()->push_back(0);
	meshManager->Add("Point")->Create(&graphicsDevice, testMeshData6);

	//���f���ǂݍ���
	MeshData<VertexInfo::Vertex_UV_Normal> testModel;
	MeshCreater::LoadModelData("paimon", testModel);
	meshManager->Add("testModel")->Create(&graphicsDevice, testModel);

	//���f���ǂݍ���
	MeshData<VertexInfo::Vertex_UV_Normal> skydomeModel;
	MeshCreater::LoadModelData("skydome", skydomeModel);
	meshManager->Add("skydome")->Create(&graphicsDevice, skydomeModel);

	////���f���ǂݍ���
	//MeshData<VertexInfo::Vertex_UV_Normal> testG3M;
	//MeshCreater::LoadGates3DModelData("testModel", testG3M);
	//meshManager->Add("testG3M")->Create(&graphicsDevice, testG3M);



	shadowRenderTex.Create(&graphicsDevice, { 1920,1080 });
	shadowDepthTex.Create(&graphicsDevice, { 1920,1080 });
	resultRenderTex.Create(&graphicsDevice, { 1920,1080 }, GatesEngine::Math::Vector4(1, 1, 1, 255));
	resultDepthTex.Create(&graphicsDevice, { 1920,1080 });
	lateDrawResultRenderTex.Create(&graphicsDevice, { 1920,1080 }, GatesEngine::Math::Vector4(141, 219, 228, 255));
	lateDrawResultDepthTex.Create(&graphicsDevice, { 1920,1080 });
	resultRenderShadowTex.Create(&graphicsDevice, { 1920,1080 }, GatesEngine::Math::Vector4(1, 1, 1, 255));
	parlinNoiseTex.Create(&graphicsDevice, { 1920,1080 });
	parlinNoiseHeightMapTex.Create(&graphicsDevice, { 1920,1080 });

	sceneManager->AddScene(new SampleScene("SampleScene", this));
	sceneManager->AddScene(new TitleScene("TitleScene", this));
	sceneManager->AddScene(new Stage1Scene("Stage1Scene", this));
	sceneManager->ChangeScene("Stage1Scene");
	sceneManager->ChangeScene("TitleScene");
	//sceneManager->ChangeScene("SampleScene");


	mainCamera = &camera;

	GameObject* playerObject = sceneManager->GetCurrentScene()->GetGameObjectManager()->Find("player");
	if (playerObject)mainCamera = playerObject->GetComponent<PlayerBehaviour>()->GetSetCamera();
	else mainCamera = &camera;

	mainCamera->SetGraphicsDevice(&graphicsDevice);
	mainCamera->SetMainWindow(&mainWindow);
	graphicsDevice.SetMainCamera(mainCamera);

	return true;
}

bool Game::Initialize()
{
	gameObjectManager.Start();
	sceneManager->GetCurrentScene()->Initialize();
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

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("CreateParlinNoiseTextureShader")->Set();
	//graphicsDevice.ClearRenderTargetOutDsv({ 0,0,0,1 }, true, &parlinNoiseTex);
	graphicsDevice.SetMultiRenderTarget({ &parlinNoiseTex,&parlinNoiseHeightMapTex }, nullptr, { 0,0,0,1 });
	using namespace GatesEngine::Math;
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({ 10 }) * Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(1, Vector4(1));
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(2, Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, Vector4(timer.GetElapsedApplicationTime()));
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();

	//�V���h�E�}�b�v�p�[�x�`��
	graphicsDevice.ClearRenderTarget({ 0,0,0,1 }, true, &shadowRenderTex, &shadowDepthTex);
	shaderManager->GetShader("DefaultMeshShader")->Set();

	//�v���C���[�̏�󂩂��x�`��
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

	//�V�[���̕`��
	sceneManager->Draw();


	graphicsDevice.SetMultiRenderTarget({ &resultRenderTex,&resultRenderShadowTex }, &resultDepthTex, GatesEngine::Math::Vector4(1, 1, 1, 255));

	//using namespace GatesEngine::Math;
	//GatesEngine::ResourceManager::GetShaderManager()->GetShader("Texture")->Set();
	//graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({ 100 }) * Matrix4x4::RotationX(ConvertToRadian(-90)) * Matrix4x4::Translate({ 0,5000,1000 }));
	//mainCamera->Set(2);
	////graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, Vector4(timer.GetElapsedApplicationTime()/10));
	//parlinNoiseTex.Set(5);
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();

	//�[�x�e�N�X�`���𗘗p���ăv���C���[���_�ŕ`��
	shaderManager->GetShader("testMultiRTVShader")->Set();

	mainCamera->Set(2);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });
	GatesEngine::Math::Matrix4x4 lightViewMatrix = lightViewData.viewMatrix * lightViewData.projMatrix;
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	shadowDepthTex.Set(5);

	//�V�[���̕`��
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	shadowDepthTex.Set(5);
	sceneManager->Draw();

	//�O���b�h�̕`��
	graphicsDevice.ClearRenderTarget(GatesEngine::Math::Vector4(141, 219, 228, 255), true, &lateDrawResultRenderTex, &lateDrawResultDepthTex);
	shaderManager->GetShader("Line")->Set();
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,0,0 }));
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
	meshManager->GetMesh("Grid")->Draw();

	//�X�v���C�g��R���C�_�[�̃��C���[�t���[���\��
	//gameObjectManager.LateDraw();
	sceneManager->LateDraw();

	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::P))
	{
		shaderManager->GetShader("TestTesselationShader")->Set(true);
	}
	else
	{
		shaderManager->GetShader("TestTesselationShader")->Set(false);
	}
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({ 100 }) * Matrix4x4::RotationX(ConvertToRadian(-90)) * Matrix4x4::Translate({ 0,5000,1000 }));
	mainCamera->Set(2);
	graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, GatesEngine::B3{ Vector4(),Vector4(0,10000,10000,1) });
	parlinNoiseTex.Set(5);
	parlinNoiseHeightMapTex.Set(6);

	meshManager->GetMesh("DividePlane")->Draw();

	//�`�挋�ʂ���[�x�e�N�X�`���𗘗p���ăA�E�g���C���t�^���Ă�`��
	graphicsDevice.ClearRenderTarget({ 0,0,0,1 }, true);

	shaderManager->GetShader("PostEffect_OutlineShader")->Set();
	graphicsDevice.GetCBVSRVUAVHeap()->Set();
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