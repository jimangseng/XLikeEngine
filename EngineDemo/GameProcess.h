#pragma once

#include <windows.h>

#include "XLEngine/XLEngine.h"

/// <summary>
/// 초기화부터 쌓아올려본다.
/// 2023. 06. 09. jimangseng
/// </summary>

class XLEngine;
class XLGamePlayEngine;

class GameProcess
{
public:
	GameProcess();
	~GameProcess();

	// 게임 초기화. 윈도우를 등록 및 생성하고, 그래픽스 엔진을 초기화 한 후, 화면에 띄운다.
	HRESULT Initialize(HINSTANCE hInstance);

	// 게임을 정리한다.
	void Finalize();

	// 게임 루프. 윈도우 메세지를 검사 후 있으면 실행, 없으면 업데이트 및 렌더 수행.
	void Loop();

	// 윈도우 메세지를 받았을 경우 실행되는 콜백 (메세지 핸들러)
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);	// 왜 static으로 해야하는가? 스태틱 안하면 윈도우 구조체 넣는 부분에서 빨간줄 뜬다.

private:

	const int m_WindowPosX = 300;
	const int m_WindowPosY = 300;

	const int m_ScreenWidth = 1200;
	const int m_ScreenHeight = 900;

	HRESULT SetupWin32(HINSTANCE hInstance);

	// 윈도 관련
	HWND m_hWnd;
	MSG m_Msg;

	// 엔진
	XLEngine* m_pXLEngine;
	
	// 게임프로세스에서 렌더러를 알고 있는 것이 합리적인가?
	// 대충 생각해봐도 아니다..
	// 알고 있어야 하는 이유는 마우스 입력 때문이다.
	// 마우스 입력을 처리하는 부분이 렌더러 안에 있는 것이 합리적인가?
	// 역시 아니다..
	// 마우스 입력 처리하는 부분은 게임 엔진 안에 있어야 한다.
	// TODO: 마우스 입력하는 부분 게임 엔진으로 옮기기
};

