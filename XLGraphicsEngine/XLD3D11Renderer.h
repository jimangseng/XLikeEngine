#pragma once

#include <vector>
#include "Utils/D3DHeaders.h"
#include <DirectXMath.h>

#include "XLCamera.h"

#include "XLEngine/GameTimer.h"

//#include "Objects.h"
class MeshObject;
class HelperObject;
class IRenderableObject;

class XLEngine;
class XLGamePlayEngine;
class XLGraphicsEngine;

class CASEParser;

class GameTimer;

class XLParticleSystem;
class XLParticleEffect;

class GBuffer;
class Quad;

struct ID3DX11Effect;

class XLD3D11Renderer
{
public:
	XLD3D11Renderer(ID3D11Device* _device, ID3D11DeviceContext* _context);
	~XLD3D11Renderer();

	// Renderer 초기화 및 정리
	bool Initialize(HWND hWnd);
	void Finalize();

	// Render
	void BeginRender();
	void Render(std::vector<IRenderableObject*>& _renderQueue);
	void EndRender();

	// Light
	void BuildLightFX();

	ID3DX11Effect* CompileFX(LPCWSTR fxFilePath);
	ID3D11ShaderResourceView* LoadTexture(LPCWSTR textureFilePath);

	// Window Handle
	HWND m_hWnd;

	// Device & Device Context
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	// Input Layout
	ID3D11InputLayout* m_pInputLayout;

	// VB, IB
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	/// SwapChain 관련
	IDXGISwapChain1* m_pSwapChain;
	ID3D11Texture2D* m_pBackBuffer;
	ID3D11RenderTargetView* m_pMainRenderTargetView;
	IDXGIFactory2* m_pFactory;	// SwapChain 생성을 위한 Factory
	bool CreateSwapChain(HWND hWnd); // SC 생성 함수

	// Render Target View 관련
	bool CreateMainRenderTargetView();

	// Depth-Scencil Buffer 관련
	ID3D11Texture2D* m_pDepthStencilBuffer;
	ID3D11Texture2D* m_pDepthStencilBuffer_2;
	ID3D11DepthStencilState* m_pDepthStencilState;
	ID3D11DepthStencilView* m_pDepthStencilView;
	bool CreateDepthStencilViewandState();

	// FX Framework 관련
	ID3DX11Effect* m_pFX;
	ID3DX11EffectTechnique* m_pTech;

	// RenderState
	ID3D11RasterizerState* m_pSolidRenderState;
	ID3D11RasterizerState* m_pWireFrameRenderState;
	void CreateRenderStates();

	// ASE Parser 
	CASEParser* m_pASEParser;

	// Camera
	XLCamera* m_pCamera;

	// Timer
	GameTimer* m_pTimer;

	XLEngine* m_pEngine;



	/// MRT 관련
	
	std::vector<GBuffer*> m_GBuffers;
	static const unsigned int NumGBuffers = 3;
	ID3D11RenderTargetView* m_GBufferRTVs[NumGBuffers];
	ID3D11RenderTargetView* m_NullRTVs[NumGBuffers];
	XLGraphicsEngine* m_pGraphicsEngine;



	/// Particle System

	//std::vector<XLParticleEffect*> m_ParticleEffects;
	XLParticleSystem* m_pParticleSystem;

	ID3D11ShaderResourceView* DSBSRV;
};

