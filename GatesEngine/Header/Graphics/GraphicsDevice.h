#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include "..\Math\Vector2.h"
#include "..\Math\Vector4.h"
#include "..\Window\Window.h"

namespace GatesEngine
{
	class RenderTarget;
	class CBVSRVUAVHeap;
	class CBufferAllocater;
	class DepthStencil;
	class Camera;
	class GPUParticleManager;
	class GraphicsDevice
	{
	private:
		using Vector2 = GatesEngine::Math::Vector2;
		using Vector4 = GatesEngine::Math::Vector4;

		D3D12_VIEWPORT               mViewport;
		D3D12_RECT                   mRect;
		IDXGIFactory6* mDxgiFactory;
		std::vector<IDXGIAdapter*>   mEnumAdapter;
		IDXGIAdapter* mUseAdapter;
		ID3D12Device* mDevice;
		ID3D12CommandAllocator* mCmdAlloc;
		ID3D12GraphicsCommandList* mCmdList;
		ID3D12CommandQueue* mCmdQueue;
		IDXGISwapChain4* mSwapChain;
		std::vector<ID3D12Resource*> mFrameBuffer;
		RenderTarget* mRenderTarget;
		ID3D12DescriptorHeap* mDsvHeap;
		ID3D12Resource* mDepthBuffer;
		ID3D12Fence* mFence;
		UINT64                       mFenceValue;

		CBVSRVUAVHeap* cbvSrvUavHeap;
		CBufferAllocater* cBufferAllocater;

		Camera* mainCamera;
		GPUParticleManager* particleManager;
	private:
		void CreateDxgiFactory();
		void CreateDevice();
		void CreateCmdList();
		void CreateSwapChain(HWND hwnd);
		void CreateRtv();
		void CreateDsv();
		void CreateFence();
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		GraphicsDevice();
		~GraphicsDevice();

		/// <summary>
		/// �e��f�o�C�X�����֐�
		/// </summary>
		/// <param name="mainWindow">���C���E�B���h�E�̃|�C���^</param>
		/// <returns></returns>
		bool Create(Window* mainWindow);

		/// <summary>
		/// �����_�\�^�[�Q�b�g�Ɛ[�x�o�b�t�@�̏����N���A���閽�߂��R�}���h���X�g�ɃZ�b�g����֐�
		/// </summary>
		/// <param name="color">�w��F�ŉ�ʂ�h��Ԃ�</param>
		/// <param name="clearFlag">�����_�\�^�[�Q�b�g�Ɛ[�x�o�b�t�@���N���A���邩�̃t���O</param>
		/// <param name="renderTarget">RenderTarget�N���X�̃|�C���^�Anullptr�̏ꍇ���̃C���X�^���X���ێ����Ă���o�b�N�o�b�t�@���Z�b�g�����</param>
		/// <param name="depthStencil">DepthStencil�N���X�̃|�C���^�Anullptr�̏ꍇ���̃C���X�^���X���ێ����Ă���[�x�o�b�t�@���Z�b�g�����</param>
		void ClearRenderTarget(const Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* renderTarget = nullptr, DepthStencil* depthStencil = nullptr);

		/// <summary>
		/// �����_�\�^�[�Q�b�g�̏����N���A���閽�߂��R�}���h���X�g�ɃZ�b�g����֐�
		/// </summary>
		/// <param name="color">�w��F�œh��Ԃ�</param>
		/// <param name="clearFlag">�N���A���邩�̃t���O</param>
		/// <param name="renderTarget">RenderTarget�N���X�̃|�C���^�Anullptr�̏ꍇ���̃C���X�^���X���ێ����Ă���o�b�N�o�b�t�@���Z�b�g�����</param>
		void ClearRenderTargetOutDsv(const Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* renderTarget = nullptr);

		/// <summary>
		/// ���̃C���X�^���X���ێ����Ă���[�x�o�b�t�@���N���A����
		/// </summary>
		void ClearDepthStencil();

		/// <summary>
		/// �R�}���h���X�g�ɐς܂�Ă���R�}���h�����Ɏ��s���Ă����A�܂����̖��߂��I���܂ŃA�v���P�[�V������ҋ@����������
		/// </summary>
		/// <returns>����</returns>
		bool ScreenFlip();

		/// <summary>
		/// resource�Ƃ��Ĉ����ɓn�����o�b�t�@�̈����������S�ɕύX����֐�
		/// </summary>
		/// <param name="resource">�w��o�b�t�@</param>
		/// <param name="before">���݂̏��</param>
		/// <param name="after">�V�������</param>
		void SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/// <summary>
		/// �r���[�|�[�g���Z�b�g����֐�
		/// </summary>
		/// <param name="size">�T�C�Y</param>
		/// <param name="pos">�����ʒu</param>
		void SetViewport(const Vector2& size, const Vector2& pos = {});

		/// <summary>
		/// �����_�\�^�[�Q�b�g�𕡐��Z�b�g����֐�
		/// </summary>
		/// <param name="renderTargets">�Z�b�g���郌���_�\�^�[�Q�b�g�̔z��  ��) {renderTarget1,renderTarget2}</param>
		/// <param name="depthStencil">�Z�b�g����[�x�o�b�t�@�Anullptr�̏ꍇ���̃C���X�^���X���ێ����Ă���[�x���Z�b�g�����</param>
		/// <param name="clearColor">�w��F�h��Ԃ�</param>
		void SetMultiRenderTarget(std::vector<RenderTarget*> renderTargets, DepthStencil* depthStencil = nullptr, const Math::Vector4& clearColor = {0,0,0,1});

		/// <summary>
		/// ���C���J�����̃|�C���^���Z�b�g����֐�
		/// </summary>
		/// <param name="camera"></param>
		void SetMainCamera(Camera* camera);

		/// <summary>
		/// ID3D12Device�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>ID3D12Device�̃|�C���^</returns>
		ID3D12Device* GetDevice();

		/// <summary>
		/// ID3D12GraphicsCommandlist�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>ID3D12GraphicsCommandlist�̃|�C���^</returns>
		ID3D12GraphicsCommandList* GetCmdList();

		/// <summary>
		/// IDXGISwapChain4�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>IDXGISwapChain4��Ԃ��֐�</returns>
		IDXGISwapChain4* GetSwapChain();

		/// <summary>
		/// �t���[���o�b�t�@�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�t���[���o�b�t�@�̃|�C���^</returns>
		ID3D12Resource* GetCurrentFrameBuffer();

		/// <summary>
		/// �[�x�o�b�t�@�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�[�x�o�b�t�@�̃|�C���^</returns>
		ID3D12Resource* GetDepthBuffer();

		/// <summary>
		/// RenderTarget�p��DescriptorHeap�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>RenderTarget�p��DescriptorHeap�̃|�C���^</returns>
		ID3D12DescriptorHeap* GetRtvHeap();

		/// <summary>
		/// CBVSRVUAV�N���X�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>CBVSRVUAV�N���X�̃|�C���^</returns>
		CBVSRVUAVHeap* GetCBVSRVUAVHeap();

		/// <summary>
		/// �萔�o�b�t�@�A���P�[�^�[�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�萔�o�b�t�@�A���P�[�^�[�̃|�C���^</returns>
		CBufferAllocater* GetCBufferAllocater();

		/// <summary>
		/// ���C���J�����̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>���C���J�����̃|�C���^</returns>
		Camera* GetMainCamera();

		GPUParticleManager* GetParticleManager();
	};
}
