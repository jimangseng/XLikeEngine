#pragma once

class XLGameplayEngine;
class XLGraphicsEngine;
class XLD3D11Renderer;
class ObjectManager;

/// <summary>
/// �ʱ�ȭ���� �׾ƿ÷�����.
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

	// �� �������� �׸���.
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
