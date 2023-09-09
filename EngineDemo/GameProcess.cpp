#pragma once

#include <windows.h>

#include "imgui/imgui.h"

#include "GameProcess.h"
#include "XLEngine/XLEngine.h"
#include "XLEngine/XLGamePlayEngine.h" // TODO: 

#pragma comment(lib, "XLEngine.lib")
#pragma comment(lib, "XLGraphicsEngine.lib")

static XLGameplayEngine* m_pXLGamePlayEngineInstance = nullptr;

// TODO: �⺻ �����ڿ� �Ҹ��ڸ� ������� ���� side-effect �����غ���..
GameProcess::GameProcess()
	:m_hWnd(), m_Msg(),
	m_pXLEngine()
{

}

GameProcess::~GameProcess()
{
	delete m_pXLEngine;
}


HRESULT GameProcess::Initialize(HINSTANCE hInstance)
{
	/// win32 ���� ����
	HRESULT hr = SetupWin32(hInstance);
	if (hr == S_FALSE)
	{
		return S_FALSE;
	}

	/// xlike ���� ����
	// xlike ���� �ʱ�ȭ
	m_pXLEngine = new XLEngine();	// TODO: ����Ʈ ������ Ȥ�� ComPtr�� ����
	m_pXLEngine->Initialize(m_hWnd);

	m_pXLGamePlayEngineInstance = m_pXLEngine->getGameplayEngine(); // TODO:
	// �������μ����� �������� �˰� �ִ� ���� �ո������� �ʴ�..


	// ������ ������ ȭ�鿡 ǥ��
	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	return S_OK;
}


void GameProcess::Loop()
{
	while (true)
	{
		if (PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_Msg.message == WM_QUIT) break;
			DispatchMessage(&m_Msg);
		}
		else
		{
			m_pXLEngine->Draw();
		}
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK GameProcess::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		// TODO: �������� �ƴ� '���ӿ���'�� ���콺 �Է� ���� �Լ����� �־�� �Ѵ�.
		m_pXLGamePlayEngineInstance->OnMouseDown(/*wParam, */LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		m_pXLGamePlayEngineInstance->OnMouseUp(/*wParam, */LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_MOUSEMOVE:
		m_pXLGamePlayEngineInstance->OnMouseMove(wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


HRESULT GameProcess::SetupWin32(HINSTANCE hInstance)
{
	///
	/// Win32 ����
	///

	// ���� Ŭ����
	wchar_t szAppName[] = L"Xlike Graphics Engine Demo";
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = GameProcess::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = szAppName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// ���� Ŭ���� ���
	RegisterClassExW(&wcex);

	// ���� ����
	m_hWnd = CreateWindowW(szAppName, szAppName,
		WS_OVERLAPPEDWINDOW,
		m_WindowPosX, m_WindowPosY, m_ScreenWidth, m_ScreenHeight,
		nullptr, nullptr, hInstance, NULL);

	if (!m_hWnd) return S_FALSE;

	return S_OK;
}

void GameProcess::Finalize()
{
	m_pXLEngine->Finalize();
}