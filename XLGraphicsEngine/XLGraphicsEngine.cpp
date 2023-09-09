#pragma once

#include <windows.h>

#include "XLGraphicsEngine.h"
#include "XLD3D11Renderer.h"
#include "Objects.h"
#include "XLASEParser/CASEParser.h"

#include "Factory.h"

#include "XLUI.h"

XLGraphicsEngine::XLGraphicsEngine(HWND hWnd)
	:m_pDevice(), m_pDeviceContext(),
	m_pRenderer(), m_pEngine(),
	m_pRenderQueue(),
	m_pASEParser(),
	m_hWnd(hWnd),
	m_pFactory(),
	m_pCamera(),
	m_pTimer()
{
}

XLGraphicsEngine::~XLGraphicsEngine()
{
	for (auto&& i : m_pRenderQueue)
	{
		delete i;
	}

	delete m_pRenderer;
	delete m_pFactory;
}

void XLGraphicsEngine::Initialize()
{
	CreateDevice();

	/// Renderer 관련
	m_pRenderer = CreateRenderer(m_pDevice, m_pDeviceContext);

	if (!m_pRenderer)
	{
		return;
	}
	m_pRenderer->m_pGraphicsEngine = this;
	m_pRenderer->Initialize(m_hWnd);


	/// IMGUI 관련 셋팅
	m_pUI = new XLUI(m_hWnd, m_pDevice, m_pDeviceContext);
	m_pUI->Initialize();

	/// Render Queue에 오브젝트 추가
	// genji
	MeshObject* genji = new MeshObject(m_pDevice, m_pDeviceContext);
	m_pRenderQueue.emplace_back(genji);

	// 환경맵용 큐브
	Cubemap* cubemap = new Cubemap(m_pDevice, m_pDeviceContext);
	//m_pRenderQueue.emplace_back(cubemap);

	// 초기화
	for (auto&& e : m_pRenderQueue)
	{
		e->Initialize(m_pRenderer);
	}
	
	// TODO: Builder 도입
	// TODO: File을 Load하는 부분도 게임 엔진에 있어야 하나?
	// Load 자체는 게임엔진에서 하고, 그래픽스 엔진에서는 load된 객체를 넘겨받아 리소스 매니저로 관리하는 것이 맞지 않나?
}

bool XLGraphicsEngine::Finalize()
{
	m_pRenderer->Finalize();
	m_pUI->Finalize();

	return true;
}

XLD3D11Renderer* XLGraphicsEngine::CreateRenderer(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	// 왜? 생성자를 한번 감싼 것 뿐 아무것도 하는 일이 없지 않나?
	return new XLD3D11Renderer(_device, _context);
}

void XLGraphicsEngine::Update()
{
	/// Light 업데이트
	// TODO: 모든 Light들의 정보를 저장하는 Lights 벡터를 만들어 일괄적으로 업데이트 하도록.. 
	// TODO: 그러기 위해서는 Light 구조체? 클래스부터 만들어야 함. IRenderableObject를 상속받아야 함.
	float RotationSpeed = 10.0f;	// 회전 속도
	//XMFLOAT3 DirectionalLightDir = XMFLOAT3(cos(GetTickCount() * (RotationSpeed * 0.0001)), 0.0f, sin(GetTickCount() * (RotationSpeed * 0.0001)));
	XMFLOAT3 DirectionalLightDir = XMFLOAT3(2.0f, 1.0f, 1.0f);
	XMFLOAT3 PointLightPosition = XMFLOAT3(sin(GetTickCount() * (RotationSpeed * 0.0001)), 1.0f, cos(GetTickCount() * (RotationSpeed * 0.0001)));
	//XMFLOAT3 SpotLightPosition = XMFLOAT3(cos(GetTickCount() * (RotationSpeed * 0.0001)), 3.0f, sin(GetTickCount() * (RotationSpeed * 0.0001)));
	XMFLOAT3 SpotLightPosition = XMFLOAT3(0.0f, 0.0f, -1.0f);
}

void XLGraphicsEngine::Render()
{
	m_pRenderer->BeginRender();
	m_pRenderer->Render(m_pRenderQueue);

 	m_pUI->Render();

	m_pRenderer->EndRender();

	
}

XLD3D11Renderer* XLGraphicsEngine::GetRenderer()
{
	return m_pRenderer;
}

bool XLGraphicsEngine::CreateDevice()
{
	/// Device 생성
	HRESULT hr = D3D11CreateDevice(
		NULL,															// [in, optional]	IDXGIAdapter				*pAdapter
		D3D_DRIVER_TYPE_HARDWARE,										//					D3D_Driver_Type				DriverType
		NULL,															//					HMODULE						Software
		D3D11_CREATE_DEVICE_DEBUG,										//					UINT						Flags
		NULL,															// [in, optional]	const D3D_FEATURE_LEVEL		*pFeatureLevels
		NULL,															//					UINT						FeatureLevels
		D3D11_SDK_VERSION,												//					UINT						SDKVersion
		&m_pDevice,														// [out, optional]	ID3D11Device				**ppDevice
		NULL,															// [out, optional]	D3D_FEATUER_LEVEL			*pFeatureLevel
		&m_pDeviceContext												// [out, optional]	ID3D11DeviceContext			**ppImmediateContext
	);

	if (FAILED(hr))
	{
		return false;
	}
}