#pragma once

#include "XLGraphicsEngine.h"
#include "XLD3D11Renderer.h"
#include "DXTK/DDSTextureLoader.h"
//#include "ASEParser/CASEParser.h"

#include <DirectXMath.h>

#include "Objects.h"

#include "GBuffer.h"
#include "Quad.h"

#include "XLParticleSystem/XLParticleEffect.h"
#include "XLParticleSystem/XLParticleSystem.h"

#pragma comment(lib, "XLASEParser.lib")
#pragma comment(lib, "XLParticleSystem.lib")

using namespace DirectX;

XLD3D11Renderer::XLD3D11Renderer(ID3D11Device* _device, ID3D11DeviceContext* _context) :
	m_hWnd(),
	m_pDevice(_device), m_pDeviceContext(_context),
	m_pFactory(),
	m_pSwapChain(), m_pBackBuffer(), m_pMainRenderTargetView(),
	m_pDepthStencilBuffer(), m_pDepthStencilState(), m_pDepthStencilView(),
	m_pFX(), m_pTech(),
	m_pSolidRenderState(), m_pWireFrameRenderState(),
	m_pASEParser(),
	m_pCamera(),
	m_pTimer(),
	m_pEngine(), m_pGraphicsEngine(),
	m_pParticleSystem()
{

}

XLD3D11Renderer::~XLD3D11Renderer()
{
	/// TODO: Resource Management

	//delete m_pParticleSystem;
	//delete m_pParticleEffects[0];
}

bool XLD3D11Renderer::Initialize(HWND hWnd)
{
	m_pCamera = m_pGraphicsEngine->m_pCamera;
	m_pTimer = m_pGraphicsEngine->m_pTimer;

	HRESULT hr = NULL;

	m_hWnd = hWnd;

	// 스왑체인 생성
	CreateSwapChain(m_hWnd);

	// Render Target View 생성
	CreateMainRenderTargetView();

	// Depth-Stencil View 와 State 생성
	CreateDepthStencilViewandState();



#pragma region MRT
	/// Create and Bind Multi Render Targets
	/// MRT 관련
	/// 
	
	// GBuffer 생성하여 벡터에 추가
	m_GBuffers.emplace_back(new GBuffer(m_pDevice, m_pDeviceContext)); // 0 - Diffuse
	m_GBuffers.emplace_back(new GBuffer(m_pDevice, m_pDeviceContext)); // 1 - Position
	m_GBuffers.emplace_back(new GBuffer(m_pDevice, m_pDeviceContext)); // 2 - Normal

	m_GBuffers[0]->m_Quad->SetPosition(0.0f, 0.0f);
	m_GBuffers[1]->m_Quad->SetPosition(0.5f, 0.0f);
	m_GBuffers[2]->m_Quad->SetPosition(0.5f, 0.5f);
	
	

	for (int i = 0; i < NumGBuffers; i++)
	{
		m_GBufferRTVs[i] = m_GBuffers[i]->GetRTV();
		m_NullRTVs[i] = nullptr;
	}

#pragma endregion MRT 시도


	// RenderState 생성
	CreateRenderStates();

	m_pDeviceContext->RSSetState(m_pSolidRenderState);

	// DSB를 생성하기 위해 BackBuffer의 정보를 가져옴
	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	m_pBackBuffer->GetDesc(&backBufferDesc);

	// Viewport 구조체 생성
	CD3D11_VIEWPORT viewport(
		0.0f,
		0.0f,
		(float)backBufferDesc.Width,
		(float)backBufferDesc.Height
	);

	// Viewport 지정
	m_pDeviceContext->RSSetViewports(1, &viewport);


	//m_pFX = CompileFX(L"../FX/solid.fx");
	//m_pFX->GetVariableByName("gDSB")->AsShaderResource()->SetResource(DSBSRV);


	/// Particle System 관련

	// 파티클 시스템 생성 및 초기화
	m_pParticleSystem = new XLParticleSystem();
	m_pParticleSystem->Init(m_pDevice, m_pDeviceContext, 5000);

	// 파티클 이펙트들을 생성
	FireParticleEffect* FireParticle = new FireParticleEffect();
	SmokeParticleEffect* SmokeParticle = new SmokeParticleEffect();
	AshParticleEffect* AshParticle = new AshParticleEffect();
	RainParticleEffect* RainParticle = new RainParticleEffect();

	// 파티클 시스템에 파티클 이펙트들을 추가
	m_pParticleSystem->AddParticleEffect(FireParticle);
	m_pParticleSystem->AddParticleEffect(SmokeParticle);
	m_pParticleSystem->AddParticleEffect(AshParticle);
	m_pParticleSystem->AddParticleEffect(RainParticle);

	// 특정 파티클 이펙트의 파라미터를 변경할 수 있다.
	AshParticle->SetParticleSize(0.5f);
	RainParticle->SetParticleInterval(0.001f);

	return true;
}
 
void XLD3D11Renderer::Finalize()
{

}


void XLD3D11Renderer::BeginRender()
{


	// Clear Render Target View
	const FLOAT ColorRGBA[4] = { 0.9f, 0.6f, 0.6f, 0.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pMainRenderTargetView, ColorRGBA);

	// Clear GBuffers
	const FLOAT ColorRGBA2[4] = { 0.4f, 0.4f, 0.4f, 0.0f };
	for (auto&& e : m_GBuffers)
	{
		m_pDeviceContext->ClearRenderTargetView(e->m_RTV, ColorRGBA2);
	}

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	// DSS, RTV와 DSV를 OM에 바인딩
	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, 0);


	//
}

void XLD3D11Renderer::Render(std::vector<IRenderableObject*>& _renderQueue)
{
	//// MRT에 렌더
	m_pDeviceContext->OMSetRenderTargets(NumGBuffers, m_GBufferRTVs, m_pDepthStencilView);
	
	// 렌더 큐를 돌면서 각 RenderableObject의 Render()를 호출
	for (auto&& i : _renderQueue)
	{
		i->Render();
	}

	//// 메인 렌더 타겟에 렌더
	//m_pDeviceContext->OMSetRenderTargets(NumGBuffers, m_NullRTVs, nullptr);
	m_pDeviceContext->OMSetRenderTargets(1, &m_pMainRenderTargetView, m_pDepthStencilView);

	for (auto&& e : m_GBuffers)
	{
		e->m_Quad->m_pDiffuse->SetResource(e->GetSRV());
		e->m_Quad->Bind();
		m_pDeviceContext->DrawIndexed(6, 0, 0);
		e->m_Quad->Unbind();
	}
	//auto&& e = m_GBuffers[1];
	//e->m_Quad->m_pDiffuse->SetResource(e->GetSRV());
	//e->m_Quad->Bind();
	//m_pDeviceContext->DrawIndexed(6, 0, 0);
	//e->m_Quad->Unbind();

	// 파티클 시스템 렌더링
	//m_pParticleSystem->Update(m_pTimer->DeltaTime(), m_pCamera->ViewProj(), m_pCamera->GetPosition());
	//m_pParticleSystem->Render();

}

void XLD3D11Renderer::EndRender()
{


	// Swap Chain의 전면 버퍼와 후면 버퍼를 교체
	m_pSwapChain->Present(0, 0);

}



void XLD3D11Renderer::BuildLightFX()
{
	m_pFX = CompileFX(L"../FX/solid.fx");

	// 테크닉을 가져온다.
	m_pTech = m_pFX->GetTechniqueByIndex(0);
}

ID3DX11Effect* XLD3D11Renderer::CompileFX(LPCWSTR fxFilePath)
{
	ID3DX11Effect* pFX = nullptr;

	ID3DBlob* compiledShader = nullptr;
	D3DCompileFromFile(fxFilePath, NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_DEBUG, NULL, &compiledShader, NULL);

	// Effect 객체 생성
	HRESULT hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_pDevice, &(pFX));

	return pFX;
}

ID3D11ShaderResourceView* XLD3D11Renderer::LoadTexture(LPCWSTR textureFilePath)
{
	/// Particle 텍스쳐 생성
	ID3D11Resource* ParticleTexture = nullptr;
	ID3D11ShaderResourceView* ParticleSRV = nullptr;

	// dds 파일로부터 Shader Resource View 생성
	HRESULT hr = CreateDDSTextureFromFile(m_pDevice, textureFilePath, &ParticleTexture, &ParticleSRV);

	return ParticleSRV;
}

bool XLD3D11Renderer::CreateSwapChain(HWND hWnd)
{
	/// SwapChain Description 구조체 정의
	DXGI_SWAP_CHAIN_DESC1 SC_d;

	SC_d.Width = 0;
	SC_d.Height = 0;

	SC_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	SC_d.Stereo = FALSE;

	SC_d.SampleDesc.Count = 1;
	SC_d.SampleDesc.Quality = 0;

	SC_d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SC_d.BufferCount = 2;

	SC_d.Scaling = DXGI_SCALING_STRETCH;
	SC_d.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SC_d.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

	SC_d.Flags = NULL;

	/// SwapChain 생성
	// SwapChain을 생성하기 위해 Factory 생성
	HRESULT hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), (void**)(&m_pFactory));

	// Factory로부터 SwapChain 생성 (윈도우 핸들 이용)
	hr = m_pFactory->CreateSwapChainForHwnd(
		m_pDevice,
		hWnd,
		&SC_d,
		NULL,
		NULL,
		&(m_pSwapChain)
	);

	if (FAILED(hr))
	{
		return false;
	}
}

void XLD3D11Renderer::CreateRenderStates()
{
	// Solid
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HRESULT hr = m_pDevice->CreateRasterizerState(&solidDesc, &m_pSolidRenderState);

	// Wireframe
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	hr = m_pDevice->CreateRasterizerState(&wireframeDesc, &m_pWireFrameRenderState);
}

bool XLD3D11Renderer::CreateMainRenderTargetView()
{

	// 스왑체인의 백버퍼(Render Target)를 멤버로 지정
	HRESULT hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackBuffer);

	if (FAILED(hr))
		return false;

	//m_GBuffers.emplace_back(new GBuffer(m_pDevice, m_pDeviceContext));

	/// 백버퍼 리소스를 사용하여 렌더타겟 뷰 생성
	// 어떤 Resource를 파이프라인에 바인딩하려면 View를 생성한 후, 그 View를 바인딩해야한다.
	hr = m_pDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, &(m_pMainRenderTargetView));


}

bool XLD3D11Renderer::CreateDepthStencilViewandState()
{
	// DSB를 생성하기 위해 BackBuffer의 정보를 가져옴
	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	m_pBackBuffer->GetDesc(&backBufferDesc);


	// Depth-Stencil Buffer를 위한 Texture Resource Description 구조체 정의
	D3D11_TEXTURE2D_DESC DSB_d;
	DSB_d.Width = backBufferDesc.Width;
	DSB_d.Height = backBufferDesc.Height;
	DSB_d.MipLevels = 1;
	DSB_d.ArraySize = 1;
	//DSB_d.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DSB_d.Format = DXGI_FORMAT_D32_FLOAT;
	DSB_d.SampleDesc.Count = 1;
	DSB_d.SampleDesc.Quality = 0;
	DSB_d.Usage = D3D11_USAGE_DEFAULT;
	DSB_d.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	DSB_d.CPUAccessFlags = 0;
	DSB_d.MiscFlags = 0;

	// Depth-Stencil Buffer 생성 (엄밀히 말하면 Buffer가 아니라, Texture2D이다. Buffer와 Texture2D는 Resource의 일종이다)
	//m_pDevice->CreateBuffer(&dsbd, );
	HRESULT hr = m_pDevice->CreateTexture2D(&DSB_d, NULL, &m_pDepthStencilBuffer);

	DSB_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	DSB_d.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	hr = m_pDevice->CreateTexture2D(&DSB_d, NULL, &m_pDepthStencilBuffer_2);

	if (FAILED(hr))
	{
		return false;
	}

	// Depth-Stencil Description 구조체 정의
	D3D11_DEPTH_STENCIL_DESC DS_d;
	ZeroMemory(&DS_d, sizeof(D3D11_DEPTH_STENCIL_DESC));
	DS_d.DepthEnable = true;
	DS_d.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DS_d.DepthFunc = D3D11_COMPARISON_LESS;

	// Depth-Stencil State 생성
	// (depth-stencil state는 OM 스테이지에 depth-stencil 테스트를 수행하는 방법을 전달한다)
	m_pDevice->CreateDepthStencilState(&DS_d, &m_pDepthStencilState);

	// Depth-Stencil View 생성
	// (Resource로 View를 생성해야 파이프라인에 바인드할 수 있다)

	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, nullptr, &m_pDepthStencilView);

	// Depth-Stencil 버퍼 시각화를 위해 Shader Resource View 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC SRV_d;
	SRV_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRV_d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	
	hr = m_pDevice->CreateShaderResourceView(m_pDepthStencilBuffer_2, nullptr, &DSBSRV);

}


