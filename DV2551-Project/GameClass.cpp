#include "stdafx.h"

#include "GameClass.h"


GameClass::GameClass()
{
	
}

GameClass::~GameClass()
{
	
}


bool GameClass::Initialize(Window* pWindow)
{
	m_pD3DFactory = new D3DFactory();
	
	m_pGraphicsHighway = m_pD3DFactory->CreateGPUHighway(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, m_iBackBufferCount, 2);

	//set up swapchain with the graphics highway
	DXGI_MODE_DESC descMode = {};
	descMode.Width = pWindow->GetWidth();
	descMode.Height = pWindow->GetHeight();
	descMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	DXGI_SAMPLE_DESC descSample = {};
	descSample.Count = 1;

	DXGI_SWAP_CHAIN_DESC descSwapChain = {};
	descSwapChain.BufferCount = m_iBackBufferCount;
	descSwapChain.BufferDesc = descMode;
	descSwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descSwapChain.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	descSwapChain.OutputWindow = pWindow->GetWindowHandle();
	descSwapChain.SampleDesc = descSample;
	descSwapChain.Windowed = true;

	m_pSwapChain = m_pD3DFactory->CreateSwapChain(&descSwapChain, m_pGraphicsHighway->GetCQ());
	

	//create rtvs and descriptor heap
	m_pDHRTV = m_pD3DFactory->CreateDH(m_iBackBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, false);
	int iSizeOffsetRTV = m_pD3DFactory->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleDH = m_pDHRTV->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < m_iBackBufferCount; ++i)
	{
		DxAssert(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_ppRTV[i])));
		m_pD3DFactory->GetDevice()->CreateRenderTargetView(m_ppRTV[i], nullptr, handleDH);
		handleDH.ptr += iSizeOffsetRTV;
	}
	m_pClearColor[0] = 0.1f;
	m_pClearColor[1] = 0.5f;
	m_pClearColor[2] = 0.3f;
	m_pClearColor[3] = 1.0f;
	
	
	//set up default blend
	D3D12_RENDER_TARGET_BLEND_DESC descBlendStateRTV = {};
	descBlendStateRTV.BlendEnable = false;
	descBlendStateRTV.BlendOp = D3D12_BLEND_OP_ADD;
	descBlendStateRTV.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	descBlendStateRTV.DestBlend = D3D12_BLEND_ZERO;
	descBlendStateRTV.DestBlendAlpha = D3D12_BLEND_ZERO;
	descBlendStateRTV.LogicOp = D3D12_LOGIC_OP_NOOP;
	descBlendStateRTV.LogicOpEnable = false;
	descBlendStateRTV.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	descBlendStateRTV.SrcBlend = D3D12_BLEND_ONE;
	descBlendStateRTV.SrcBlendAlpha = D3D12_BLEND_ONE;
	
	D3D12_BLEND_DESC descBlendSate = {};
	descBlendSate.AlphaToCoverageEnable = false;
	descBlendSate.IndependentBlendEnable = false;
	for (int i = 0; i < m_iBackBufferCount; ++i)
		descBlendSate.RenderTarget[i] = descBlendStateRTV;

	D3D12_RASTERIZER_DESC descRasterizer = {};
	descRasterizer.FillMode = D3D12_FILL_MODE_SOLID;
	descRasterizer.CullMode = D3D12_CULL_MODE_BACK;
	descRasterizer.FrontCounterClockwise = false;
	descRasterizer.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	descRasterizer.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	descRasterizer.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	descRasterizer.DepthClipEnable = true;
	descRasterizer.MultisampleEnable = false;
	descRasterizer.AntialiasedLineEnable = false;
	descRasterizer.ForcedSampleCount = 0;
	descRasterizer.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;


	ID3DBlob* pVSblob = m_pD3DFactory->CompileShader(L"VertexShader.hlsl", "vs_5_1");
	ID3DBlob* pPSblob = m_pD3DFactory->CompileShader(L"PixelShader.hlsl", "ps_5_1");

	D3D12_GRAPHICS_PIPELINE_STATE_DESC descPSO = {};
	descPSO.BlendState = descBlendSate;
	//descPSO.DepthStencilState
	descPSO.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	descPSO.NumRenderTargets = 1;
	descPSO.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	descPSO.VS = { pVSblob->GetBufferPointer(), pVSblob->GetBufferSize() };
	descPSO.PS = { pPSblob->GetBufferPointer(), pPSblob->GetBufferSize() };
	descPSO.RasterizerState = descRasterizer;
	descPSO.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	descPSO.SampleDesc = descSample;
	descPSO.SampleMask = 0xffffffff;
	//descPSO.pRootSignature = m_pRS;


	SAFE_RELEASE(pVSblob);
	SAFE_RELEASE(pPSblob);

	return true;
}

void GameClass::CleanUp()
{
	if (m_pD3DFactory)
	{
		delete m_pD3DFactory;
		m_pD3DFactory = nullptr;
	}

}

void GameClass::Update(Input * input, double dDeltaTime)
{
	m_dDeltaTime = dDeltaTime;
}

void GameClass::ClearBackBuffer()
{
	int iFrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	ID3D12GraphicsCommandList* pCL = m_pGraphicsHighway->GetFreshCL();

	D3D12_RESOURCE_TRANSITION_BARRIER transition = {};
	transition.pResource = m_ppRTV[iFrameIndex];
	transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	D3D12_RESOURCE_BARRIER barrierTransition = {};
	barrierTransition.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierTransition.Transition = transition;

	pCL->ResourceBarrier(1, &barrierTransition);
	int iIncrementSizeRTV = m_pD3DFactory->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE handleDH = m_pDHRTV->GetCPUDescriptorHandleForHeapStart();
	handleDH.ptr += iIncrementSizeRTV * iFrameIndex;

	pCL->ClearRenderTargetView(handleDH, m_pClearColor, NULL, nullptr);
	pCL->OMSetRenderTargets(1, &handleDH, NULL, nullptr);
}


void GameClass::PrecentBackBuffer()
{


}

void GameClass::Frame()
{

	//for each rs
		//for each pipe
			//highway, get cls
			//for each object
}
