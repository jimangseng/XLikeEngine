#include "XLGameplayEngine.h"
#include "XLGraphicsEngine/XLCamera.h"


XLGameplayEngine::XLGameplayEngine(HWND hWnd)
	:m_hWnd(hWnd),
	m_pTimer(),
	m_pCamera(),
	mLastMousePosX(),
	mLastMousePosY(),
	m_CameraSpeed()
{
}

XLGameplayEngine::~XLGameplayEngine()
{
	delete m_pCamera;
	delete m_pTimer;
}

void XLGameplayEngine::Initialize()
{
	/// Timer 관련
	m_pTimer = new GameTimer();
	m_pTimer->Reset();

	/// Camera 관련
	CreateCamera();
	m_CameraSpeed = 5.0f;
}

void XLGameplayEngine::Update()
{

	/// 시간 계산 부분
	// 게임플레이 엔진으로 넘기기
	// 매 프레임 시간을 계산한다.
	m_pTimer->Tick();

	// 매 프레임의 deltaTime
	m_pTimer->DeltaTime();


	// 타이머를 기반으로 FPS, millisecond per frame (1프레임당 elapsedTime = deltaTime)을 계산 해 준다.

	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((m_pTimer->TotalTime() - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		/*
		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());
		*/

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}

	/// 키 입력

	if (GetAsyncKeyState(VK_UP)) {}
	if (GetAsyncKeyState(VK_DOWN)) {}
	if (GetAsyncKeyState(VK_RIGHT)) {}
	if (GetAsyncKeyState(VK_LEFT)) {}
	if (GetAsyncKeyState(VK_SPACE)) {}
	if (GetAsyncKeyState(VK_RETURN)) {}
	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	/// 카메라 업데이트
	UpdateCamera(m_pTimer->DeltaTime());
}

void XLGameplayEngine::Finalize()
{

}

void XLGameplayEngine::CreateCamera()
{
	m_pCamera = new XLCamera();
}

void XLGameplayEngine::UpdateCamera(const float& _deltaTime)
{

	// 카메라
	if (GetAsyncKeyState('W') & 0x8000)
		m_pCamera->Walk(m_CameraSpeed * _deltaTime);

	if (GetAsyncKeyState('S') & 0x8000)
		m_pCamera->Walk(-m_CameraSpeed * _deltaTime);

	if (GetAsyncKeyState('A') & 0x8000)
		m_pCamera->Strafe(-m_CameraSpeed * _deltaTime);

	if (GetAsyncKeyState('D') & 0x8000)
		m_pCamera->Strafe(m_CameraSpeed * _deltaTime);

	if (GetAsyncKeyState('Q') & 0x8000)
		m_pCamera->WorldUpDown(-m_CameraSpeed * _deltaTime);

	if (GetAsyncKeyState('E') & 0x8000)
		m_pCamera->WorldUpDown(m_CameraSpeed * _deltaTime);

	// view TM을 만든다.
	m_pCamera->UpdateViewMatrix();
}


void XLGameplayEngine::OnMouseDown(int x, int y)
{
	mLastMousePosX = x;
	mLastMousePosY = y;

	SetCapture(m_hWnd);
}

void XLGameplayEngine::OnMouseUp(int x, int y)
{
	ReleaseCapture();
}

void XLGameplayEngine::OnMouseMove(int btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f * static_cast<float>(x - mLastMousePosX));
		float dy = XMConvertToRadians(0.25f * static_cast<float>(y - mLastMousePosY));

		m_pCamera->Pitch(dy);
		m_pCamera->RotateY(dx);
	}

	mLastMousePosX = x;
	mLastMousePosY = y;
}
