#pragma once

#include <windows.h>
#include "Utils/D3DHeaders.h"

class XLUI
{
public:
	XLUI(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* dc);
	~XLUI();

public:
	void Initialize();
	void Render();
	void Finalize();

private:
	HWND m_hWnd;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
};

