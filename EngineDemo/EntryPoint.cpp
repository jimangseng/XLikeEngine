#include "GameProcess.h"

// Initial Entry Point
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// 게임 프로세스
	GameProcess* pGameProcess = new GameProcess();

	// 게임 프로세스 초기화
	pGameProcess->Initialize(hInstance);

	// 게임 루프
	pGameProcess->Loop();

	// 게임 마무리 - 그래픽스 엔진 마무리 등 작업
	pGameProcess->Finalize();

	delete pGameProcess;
}