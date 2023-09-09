#include <vector>
#include <DirectXMath.h>

#include "Utils/D3DHeaders.h"

#include "Quad.h"

static HRESULT hr = NULL;

Quad::Quad(ID3D11Device* _device, ID3D11DeviceContext* _dc)
	:m_pDevice(_device), m_pDeviceContext(_dc),
	m_Scale(1.0f), m_PositionX(0.0f), m_PositionY(0.0f)
{
	BuildFX();
	BuildBuffers();
	BuildInputLayout();
}

Quad::~Quad()
{

}

void Quad::Bind()
{
	// VB, IB 바인딩
	UINT offset = 0;

	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VB, &m_StrideVB, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_IB, DXGI_FORMAT_R32_UINT, 0);

	// InputLayout 바인딩
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 쉐이더 업데이트
	m_pTech->GetPassByIndex(0)->Apply(0, m_pDeviceContext);
}

void Quad::BuildFX()
{
	/// Effect 생성
// .fx 파일 컴파일
	ID3DBlob* compiledShader = nullptr;

	hr = D3DCompileFromFile(L"../FX/Quad.fx", NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_DEBUG, NULL, &compiledShader, NULL);

	// Effect 객체 생성
	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_pDevice, &m_pFX);

	// 테크닉을 가져온다.
	m_pTech = m_pFX->GetTechniqueByIndex(0);

	m_pDiffuse = m_pFX->GetVariableByName("gDiffuse")->AsShaderResource();
}

void Quad::BuildBuffers()
{
	//// 사각형을 그리려면?
	//float scale = 0.8f;
	//float positionX = 0.3f;
	//float positionY = 0.0f;

	float width = 0.8f;
	float height = 0.8f;

	std::vector<QuadVB> verticesData;
	verticesData.emplace_back(QuadVB{ { m_PositionX, m_PositionY }, { 0.0f, 0.0f } });
	verticesData.emplace_back(QuadVB{ { m_PositionX + width * 2, m_PositionY }, { 1.0f , 0.0f } });
	verticesData.emplace_back(QuadVB{ { m_PositionX, m_PositionY - height * 2 }, { 0.0f, 1.0f} });
	verticesData.emplace_back(QuadVB{ { m_PositionX + width * 2, m_PositionY - height * 2}, { 1.0f, 1.0f} });

	m_StrideVB = sizeof(QuadVB);

	// Buffer Description
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.ByteWidth = m_StrideVB * verticesData.size();
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA VBInitData;
	VBInitData.pSysMem = &(verticesData[0]);
	VBInitData.SysMemPitch = 0;
	VBInitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	hr = m_pDevice->CreateBuffer(&VBDesc, &VBInitData, &m_VB);
	// 버텍스 4개를 담은 버텍스 버퍼 생성

	// 인덱스 6개를 담은 인덱스 버퍼 생성

	std::vector<int> indicesData;
	indicesData.emplace_back(0);
	indicesData.emplace_back(1);
	indicesData.emplace_back(3);
	indicesData.emplace_back(0);
	indicesData.emplace_back(3);
	indicesData.emplace_back(2);

	m_StrideIB = sizeof(int);

	// Buffer Description
	D3D11_BUFFER_DESC IBDesc;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.ByteWidth = m_StrideIB * indicesData.size();
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA IBInitData;
	IBInitData.pSysMem = &(indicesData[0]);
	IBInitData.SysMemPitch = 0;
	IBInitData.SysMemSlicePitch = 0;

	// Create the Index buffer.
	hr = m_pDevice->CreateBuffer(&IBDesc, &IBInitData, &m_IB);
}

void Quad::BuildInputLayout()
{
	m_pTech = m_pFX->GetTechniqueByIndex(0);

	/// InputLayout 생성 및 바인딩
	// Input Element Description 구조체
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// InputLayout 생성
	D3DX11_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_pDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout);

}

void Quad::Unbind()
{
	m_pDiffuse->SetResource(nullptr);
	m_pTech->GetPassByIndex(0)->Apply(0, m_pDeviceContext);
}

void Quad::SetPosition(float positionX, float positionY)
{
	m_PositionX = positionX * 2 - 1;
	m_PositionY = positionY *(-2) + 1;

	BuildBuffers();
}

void Quad::SetScale(float scale)
{
	m_Scale = scale;

	BuildBuffers();
}
