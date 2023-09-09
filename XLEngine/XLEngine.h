#pragma once

class XLGameplayEngine;
class XLGraphicsEngine;
class XLD3D11Renderer;
class ObjectManager;

/// <summary>
/// 초기화부터 쌓아올려본다.
/// 2023. 06. 09. jimangseng
/// </summary>

class XLEngine
{
public:
	XLEngine();
	~XLEngine();

	/// Engine Setup
	bool Initialize(HWND hWnd);
	void Finalize();

	// 한 프레임을 그린다.
	void Draw();

	XLGameplayEngine* getGameplayEngine();

public:
	/// GamePlay and Graphics Engine
	XLGraphicsEngine* pGraphicsEngine;
	XLGameplayEngine* pGameplayEngine;
	XLGraphicsEngine* CreateGraphicsEngine(HWND hWnd);
	XLGameplayEngine* CreateGameplayEngine(HWND hWnd);

private:
	HWND m_hWnd;
};
