#pragma once

#include <windows.h>
#include <vector>
#include "Objects.h"
#include "XLCamera.h"

class XLEngine;
class XLD3D11Renderer;
class Factory;
class XLCamera;
class GameTimer;
class XLParticleEffect;
class XLParticleSystem;
class XLUI;


class XLGraphicsEngine
{
public:
	XLGraphicsEngine(HWND hWnd);
	~XLGraphicsEngine();

	void Initialize();
	bool Finalize();


	XLD3D11Renderer* CreateRenderer(ID3D11Device* _device, ID3D11DeviceContext* _context);
	XLD3D11Renderer* GetRenderer();
	void Update();
	void Render();


	// device & device context
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	// renderer
	XLD3D11Renderer* m_pRenderer;
	XLEngine* m_pEngine;

	XLCamera* m_pCamera;
	GameTimer* m_pTimer;

private:
	std::vector<IRenderableObject*> m_pRenderQueue;
	
	CASEParser* m_pASEParser;

	HWND m_hWnd;

	Factory* m_pFactory;

public:

private:
	XLUI* m_pUI;


private:
	bool CreateDevice();
};

