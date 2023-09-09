#pragma once

struct QuadVB
{
	float2 PosS;
	float2 UV;
};

class Quad
{
public:
	Quad(ID3D11Device* _device, ID3D11DeviceContext* _dc);
	~Quad();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

private:
	ID3D11Buffer* m_VB;
	ID3D11Buffer* m_IB;

private:
	ID3DX11Effect* m_pFX;
	ID3DX11EffectTechnique* m_pTech;

private:
	UINT m_StrideVB;
	UINT m_StrideIB;

private:
	ID3D11InputLayout* m_pInputLayout;

public:
	ID3DX11EffectShaderResourceVariable* m_pDiffuse;

private:
	float m_Scale;
	float m_PositionX;
	float m_PositionY;

public:
	void Bind();
	void BuildInputLayout();
	void Unbind();

public:
	void SetPosition(float positionX, float positionY);
	void SetScale(float scale);

private:
	void BuildFX();
	void BuildBuffers();
};

