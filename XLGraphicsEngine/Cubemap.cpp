#include "Cubemap.h"

#include "DXTK/DDSTextureLoader.h"

Cubemap::Cubemap(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	m_pDevice = pDevice;
	m_pDeviceContext = pDeviceContext;
}

Cubemap::~Cubemap()
{

}

void Cubemap::BuildBuffers()
{
	// BOX 하드코딩
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {-100.0f, 100.0f, -100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {100.0f, 100.0f, -100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {-100.0f, 100.0f, 100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {100.0f, 100.0f, 100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {100.0f, -100.0f, -100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {-100.0f, -100.0f, -100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {-100.0f, -100.0f, 100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });
	VBData.emplace_back(MeshVertex{ XMFLOAT3 {100.0f, -100.0f, 100.0f}, XMFLOAT3{0.0f, 0.0f, 0.0f}, XMFLOAT2{0.0f, 0.0f} });

	IBData.emplace_back(0);
	IBData.emplace_back(1);
	IBData.emplace_back(2);

	IBData.emplace_back(2);
	IBData.emplace_back(1);
	IBData.emplace_back(3);

	IBData.emplace_back(7);
	IBData.emplace_back(2);
	IBData.emplace_back(3);

	IBData.emplace_back(2);
	IBData.emplace_back(7);
	IBData.emplace_back(6);

	IBData.emplace_back(3);
	IBData.emplace_back(1);
	IBData.emplace_back(4);

	IBData.emplace_back(4);
	IBData.emplace_back(7);
	IBData.emplace_back(3);

	IBData.emplace_back(5);
	IBData.emplace_back(2);
	IBData.emplace_back(6);

	IBData.emplace_back(5);
	IBData.emplace_back(0);
	IBData.emplace_back(2);

	IBData.emplace_back(7);
	IBData.emplace_back(4);
	IBData.emplace_back(6);

	IBData.emplace_back(6);
	IBData.emplace_back(4);
	IBData.emplace_back(5);

	IBData.emplace_back(0);
	IBData.emplace_back(4);
	IBData.emplace_back(1);

	IBData.emplace_back(0);
	IBData.emplace_back(5);
	IBData.emplace_back(4);
}

void Cubemap::SetTextures()
{
	/// Cube Mapping 관련
	ID3D11Resource* CubeMapTexture = nullptr;

	hr = CreateDDSTextureFromFile(m_pDevice, L"../Textures/desertcube1024.dds", &CubeMapTexture, &m_pCubeMap);

	ID3DX11EffectShaderResourceVariable* CubeMap = m_pFX->GetVariableByName("gCubeMap")->AsShaderResource();
	hr = CubeMap->SetResource(m_pCubeMap);
}

void Cubemap::BuildFX()
{
	/// Effect 생성
	//.fx 파일 컴파일
	ID3DBlob* compiledShader = nullptr;

	hr = D3DCompileFromFile(L"../FX/Cubemap.fx", NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_DEBUG, NULL, &compiledShader, NULL);

	// Effect 객체 생성
	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_pDevice, &m_pFX);

	// 테크닉을 가져온다.
	m_pTech = m_pFX->GetTechniqueByIndex(0);
}

