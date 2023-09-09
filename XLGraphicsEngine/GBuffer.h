#pragma once

#include "Utils/D3DHeaders.h"

class Quad;

class GBuffer
{
public:
	GBuffer(ID3D11Device* device, ID3D11DeviceContext* dc);
	~GBuffer();

public:

private:
	void CreateBuffer();
	void CreateRenderTargetView(ID3D11Texture2D* Buffer);
	void CreateShaderResourceView(ID3D11Texture2D* RenderTarget);

public:
	ID3D11RenderTargetView* GetRTV();
	ID3D11ShaderResourceView* GetSRV();

private:
	ID3D11Texture2D* m_Buffer;

public:
	ID3D11RenderTargetView* m_RTV;
	ID3D11ShaderResourceView* m_SRV;
	Quad* m_Quad;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

};

