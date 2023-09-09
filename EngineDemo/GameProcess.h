#pragma once

#include <windows.h>

#include "XLEngine/XLEngine.h"

/// <summary>
/// �ʱ�ȭ���� �׾ƿ÷�����.
/// 2023. 06. 09. jimangseng
/// </summary>

class XLEngine;
class XLGamePlayEngine;

class GameProcess
{
public:
	GameProcess();
	~GameProcess();

	// ���� �ʱ�ȭ. �����츦 ��� �� �����ϰ�, �׷��Ƚ� ������ �ʱ�ȭ �� ��, ȭ�鿡 ����.
	HRESULT Initialize(HINSTANCE hInstance);

	// ������ �����Ѵ�.
	void Finalize();

	// ���� ����. ������ �޼����� �˻� �� ������ ����, ������ ������Ʈ �� ���� ����.
	void Loop();

	// ������ �޼����� �޾��� ��� ����Ǵ� �ݹ� (�޼��� �ڵ鷯)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// �� static���� �ؾ��ϴ°�? ����ƽ ���ϸ� ������ ����ü �ִ� �κп��� ������ ���.

private:

	const int m_WindowPosX = 300;
	const int m_WindowPosY = 300;

	const int m_ScreenWidth = 1200;
	const int m_ScreenHeight = 900;

	HRESULT SetupWin32(HINSTANCE hInstance);

	// ���� ����
	HWND m_hWnd;
	MSG m_Msg;

	// ����
	XLEngine* m_pXLEngine;
	
	// �������μ������� �������� �˰� �ִ� ���� �ո����ΰ�?
	// ���� �����غ��� �ƴϴ�..
	// �˰� �־�� �ϴ� ������ ���콺 �Է� �����̴�.
	// ���콺 �Է��� ó���ϴ� �κ��� ������ �ȿ� �ִ� ���� �ո����ΰ�?
	// ���� �ƴϴ�..
	// ���콺 �Է� ó���ϴ� �κ��� ���� ���� �ȿ� �־�� �Ѵ�.
	// TODO: ���콺 �Է��ϴ� �κ� ���� �������� �ű��
};

