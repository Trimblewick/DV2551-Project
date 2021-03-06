#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include "GPUHighway.h"
#include "Camera.h"
#include "BezierClass.h"

#include "Plane.h"

class D3DFactory
{
public:
	D3DFactory();
	~D3DFactory();

	ID3D12Device*							GetDevice();

	ID3D12CommandQueue*						CreateCQ(D3D12_COMMAND_LIST_TYPE type);
	ID3D12CommandAllocator*					CreateCA(D3D12_COMMAND_LIST_TYPE type);
	ID3D12GraphicsCommandList*				CreateCL(ID3D12CommandAllocator* pCA, D3D12_COMMAND_LIST_TYPE type);//created in recording state
	ID3D12Fence*							CreateFence();
	ID3D12RootSignature*					CreateRS(D3D12_ROOT_SIGNATURE_DESC* pDesc);
	ID3D12PipelineState*					CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC* pDesc);
	IDXGISwapChain3*						CreateSwapChain(DXGI_SWAP_CHAIN_DESC* pDesc, ID3D12CommandQueue* pCQ);
	ID3D12DescriptorHeap*					CreateDH(int numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type, bool bShaderVisible);
	ID3DBlob*								CompileShader(LPCWSTR filePath, LPCSTR shadermodel);
	void									UpdateSubresources(ID3D12GraphicsCommandList* pCmdList, ID3D12Resource* pBufferHeap, ID3D12Resource* pUploadHeap, D3D12_SUBRESOURCE_DATA* pData);
	ID3D12Resource*							CreateCommitedResource(D3D12_HEAP_TYPE type, D3D12_RESOURCE_DESC* pResourceDesc, D3D12_RESOURCE_STATES initState, D3D12_CLEAR_VALUE* clearValue = nullptr);
	GPUHighway*								CreateGPUHighway(D3D12_COMMAND_LIST_TYPE type, unsigned int iNumberOfCLs);
	Camera*									CreateCamera(unsigned int iBufferCount, long iWidthWindow, long iHeightWindow);
	BezierClass*							CreateBezier(int iWidthPlane, unsigned int iBufferCount);
	Plane*									CreatePlane(ID3D12GraphicsCommandList* pCmdList, unsigned int tiles, ID3D12Resource* pUploadHeapVertexBuffer, ID3D12Resource* pUploadHeapIndexBuffer);

private:
	IDXGIFactory4*							m_pDXGIFactory;
	ID3D12Device*							m_pDevice;
};
