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


//TODO: Engine�� �߻�ȭ�� �������̽� ������ ������ ��

XLEngine::XLEngine()
	: m_hWnd(NULL),
	pGameplayEngine(),
	pGraphicsEngine()
{

}

XLEngine::~XLEngine()
{
	// ���Ŀ� ����Ʈ�����ͷ� �����ϱ�
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
	// TODO: �ʿ��Ѱ�? �ϴ� ���� ���� ���� �� �ܰ� �� ��ġ�⸸ �� ���ε�..
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


	// TODO: ������� View Proj ����� �����ϴ� ī�޶� ��ü�� ������ ������ �������� �� �ִ� ����� �����غ��ƾ� �Ѵ�.
	// ����?
	// render
	pGraphicsEngine->Render();
}

XLGameplayEngine* XLEngine::getGameplayEngine()
{
	return pGameplayEngine;
}
