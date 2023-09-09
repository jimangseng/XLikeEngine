#pragma once

#include <windows.h>

#include "XLEngine/GameTimer.h"

class XLCamera;

class XLGameplayEngine
{
public:
	XLGameplayEngine(HWND hWnd);
	~XLGameplayEngine();

	void Initialize();
	void Update();
	void Finalize();

	// 윈도우 핸들
	HWND m_hWnd;

	// 시간 제어
	GameTimer* m_pTimer;

	// camera
	XLCamera* m_pCamera;
	void CreateCamera();
	void UpdateCamera(const float& _deltaTime);
	float m_CameraSpeed;

	// mouse
	void OnMouseDown(int x, int y);
	void OnMouseUp(int x, int y);
	void OnMouseMove(int btnState, int x, int y);

	LONG mLastMousePosX;
	LONG mLastMousePosY;
};

