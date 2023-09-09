#include <iostream>

#include "Utils/D3DHeaders.h"
#include "XLEngine.h"
#include "XLGraphicsEngine/XLGraphicsEngine.h"
#include "XLGraphicsEngine/XLD3D11Renderer.h"
#include "XLGameplayEngine.h"

#ifdef _DEBUG
#pragma comment (lib, "DirectXTK_debug.lib")
#else
#pragma comment (lib, "DirectXTK.lib")
#endif


//TODO: Engine을 추상화한 인터페이스 구조로 수정할 것

XLEngine::XLEngine()
	: m_hWnd(NULL),
	pGameplayEngine(),
	pGraphicsEngine()
{

}

XLEngine::~XLEngine()
{
	// 추후에 스마트포인터로 변경하기
	delete pGraphicsEngine;
	delete pGameplayEngine;
}

bool XLEngine::Initialize(HWND hWnd)
{

	/// Gameplay Engine Setup
	pGameplayEngine = CreateGameplayEngine(hWnd);
	pGameplayEngine->Initialize();

	/// Graphics Engine Setup
	pGraphicsEngine = CreateGraphicsEngine(hWnd);
	pGraphicsEngine->m_pCamera = pGameplayEngine->m_pCamera;
	pGraphicsEngine->m_pTimer = pGameplayEngine->m_pTimer;

	pGraphicsEngine->Initialize();
	pGraphicsEngine->m_pEngine = this;

	return true;
}

void XLEngine::Finalize()
{
	pGraphicsEngine->Finalize();
	pGameplayEngine->Finalize();
}

XLGraphicsEngine* XLEngine::CreateGraphicsEngine(HWND hWnd)
{
	// TODO: 필요한가? 하는 일이 없고 단지 한 단계 더 거치기만 할 뿐인데..
	return new XLGraphicsEngine(hWnd);

}

XLGameplayEngine* XLEngine::CreateGameplayEngine(HWND hWnd)
{
	return new XLGameplayEngine(hWnd);
}



void XLEngine::Draw()
{
	// update
	pGameplayEngine->Update();


	// TODO: 만들어진 View Proj 행렬을 포함하는 카메라 객체를 렌더러 안으로 던져넣을 수 있는 방법을 생각해보아야 한다.
	// 빌더?
	// render
	pGraphicsEngine->Render();
}

XLGameplayEngine* XLEngine::getGameplayEngine()
{
	return pGameplayEngine;
}
