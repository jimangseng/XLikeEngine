#include "MeshObject.h"
#include "XLD3D11Renderer.h"
#include "XLEngine/XLEngine.h"
#include "XLEngine/XLGameplayEngine.h"
#include "XLGraphicsEngine.h"
#include "DXTK/DDSTextureLoader.h"
#include "XLASEParser/CASEParser.h"

MeshObject::MeshObject(ID3D11Device* _device, ID3D11DeviceContext* _context)
	:m_pDevice(_device), m_pDeviceContext(_context)
{

}

MeshObject::MeshObject()
{

}

MeshObject::~MeshObject()
{

}

void MeshObject::Initialize(XLD3D11Renderer* _renderer)
{
	m_pRenderer = _renderer;

	BuildFX();

	BuildBuffers();

	m_pVertexBuffer = CreateVertexBuffer(VBData, VBData.size() * sizeof(MeshVertex));
	m_pIndexBuffer = CreateIndexBuffer(IBData, IBData.size() * sizeof(int));
	m_NumberofIndices = IBData.size();

	BuildInputLayout();

	// InputLayout ���ε�
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	SetTextures();
}

void MeshObject::Finalize()
{

}

void MeshObject::BuildFX()
{
	/// Effect ����
	// .fx ���� ������
	ID3DBlob* compiledShader = nullptr;

	//hr = D3DCompileFromFile(L"../FX/solid.fx", NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_DEBUG, NULL, &compiledShader, NULL);
	hr = D3DCompileFromFile(L"../FX/Deferred.fx", NULL, NULL, NULL, "fx_5_0", D3DCOMPILE_DEBUG, NULL, &compiledShader, NULL);

	// Effect ��ü ����
	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, m_pDevice, &m_pFX);

	// ��ũ���� �����´�.
	m_pTech = m_pFX->GetTechniqueByIndex(0);
}

void MeshObject::BuildInputLayout()
{
	/// InputLayout ���� �� ���ε�
	// Input Element Description ����ü
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// InputLayout ����
	D3DX11_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	hr = m_pDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout);
}

void MeshObject::BuildBuffers()
{
	ParseMesh("../ASEFile/genji_max.ASE");
}

void MeshObject::SetTextures()
{
	/// Diffuse �ؽ��� ����
	ID3D11Resource* DiffuseTexture = nullptr;

	// dds ���Ϸκ��� Shader Resource View ����
	hr = CreateDDSTextureFromFile(m_pDevice, L"../Textures/Genji_Reverse.dds", &DiffuseTexture, &m_pDiffuse);

	ID3DX11EffectShaderResourceVariable* Diffuse = m_pFX->GetVariableByName("gDiffuse")->AsShaderResource();
	hr = Diffuse->SetResource(m_pDiffuse);

		/// Cube Mapping ����
	ID3D11Resource* CubeMapTexture = nullptr;

	hr = CreateDDSTextureFromFile(m_pDevice, L"../Textures/desertcube1024.dds", &CubeMapTexture, &m_pCubeMap);

	ID3DX11EffectShaderResourceVariable* CubeMap = m_pFX->GetVariableByName("gCubeMap")->AsShaderResource();
	hr = CubeMap->SetResource(m_pCubeMap);
}

ID3D11Buffer* MeshObject::CreateVertexBuffer(vector<MeshVertex> verticesData, UINT sizeInByte)
{
	// Buffer Description
	D3D11_BUFFER_DESC VBDesc;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.ByteWidth = sizeInByte;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA VBInitData;
	VBInitData.pSysMem = &(verticesData[0]);
	VBInitData.SysMemPitch = 0;
	VBInitData.SysMemSlicePitch = 0;

	ID3D11Buffer* VB;

	// Create the vertex buffer.
	HRESULT hr = m_pDevice->CreateBuffer(&VBDesc, &VBInitData, &VB);

	return VB;
}

ID3D11Buffer* MeshObject::CreateIndexBuffer(vector<int> indicesData, UINT sizeInByte)
{
	// Buffer Description
	D3D11_BUFFER_DESC IBDesc;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.ByteWidth = sizeInByte;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;

	// Subresource Data
	D3D11_SUBRESOURCE_DATA IBInitData;
	IBInitData.pSysMem = &(indicesData[0]);
	IBInitData.SysMemPitch = 0;
	IBInitData.SysMemSlicePitch = 0;

	ID3D11Buffer* IB;

	// Create the Index buffer.
	HRESULT hr = m_pDevice->CreateBuffer(&IBDesc, &IBInitData, &IB);

	return IB;
}


void MeshObject::Render()
{

	XLCamera* pCamera = m_pRenderer->m_pCamera;

	// TODO: �����÷��� �������� �������� ���� ������Ʈ���� ī�޶� ��ü(VP matrix)�� ��� �Ѱ��� ���ΰ�?
	/// WVP ��� ���̴��� ����
	//world = XMMatrixIdentity();
	world = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	view = pCamera->View();
	proj = pCamera->Proj();

	WVP = world * view * proj;

	m_pFX->GetVariableByName("gWorldViewProj")->AsMatrix()->SetMatrix(reinterpret_cast<const float*>(&WVP));


	/// Light
	// TODO: �и��ϱ�..
	float RotationSpeed = 10.0f;	// ȸ�� �ӵ�
	//XMFLOAT3 DirectionalLightDir = XMFLOAT3(cos(GetTickCount() * (RotationSpeed * 0.0001)), 0.0f, sin(GetTickCount() * (RotationSpeed * 0.0001)));
	XMFLOAT3 DirectionalLightDir = XMFLOAT3(2.0f, 1.0f, 1.0f);
	XMFLOAT3 PointLightPosition = XMFLOAT3(sin(GetTickCount() * (RotationSpeed * 0.0001)), 1.0f, cos(GetTickCount() * (RotationSpeed * 0.0001)));
	//XMFLOAT3 SpotLightPosition = XMFLOAT3(cos(GetTickCount() * (RotationSpeed * 0.0001)), 3.0f, sin(GetTickCount() * (RotationSpeed * 0.0001)));
	XMFLOAT3 SpotLightPosition = XMFLOAT3(0.0f, 0.0f, -1.0f);

	/// TODO: MRT�� ���� ��� �ּ� ó�� 23.09.05
	m_pFX->GetVariableByName("gDirectionalLightDir")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&DirectionalLightDir));
	m_pFX->GetVariableByName("gPointLightPosition")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&PointLightPosition));
	m_pFX->GetVariableByName("gSpotLightPosition")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&SpotLightPosition));


	XMFLOAT3 EYE = pCamera->GetPosition(); // specular ����� ���� ���� ����

	// ���̴���..
	m_pFX->GetVariableByName("gEyePosition")->AsVector()->SetFloatVector(reinterpret_cast<const float*>(&EYE));



	UINT stride = sizeof(MeshVertex);
	UINT offset = 0;

	// PrimitiveTopology ����
	m_pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	// ���۸� ���ε�
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);


	// Technique
	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTech->GetDesc(&techDesc);

	// Render Pass
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTech->GetPassByIndex(p)->Apply(0, m_pDeviceContext);
		m_pDeviceContext->DrawIndexed(m_NumberofIndices, 0, 0);
	}

}

void MeshObject::ParseMesh(const char* ASEFilePath)
{
	CASEParser* _parser = new CASEParser();
	_parser->Parse(ASEFilePath);

	unsigned int Offset = 0;

	// �޽��� ����Ʈ�� ���� ���ؽ� �ε��� �������� �ϳ��� �������ش�.
	for (auto e : _parser->m_MeshList)
	{
		// convertALL�� ����: �޽� ����ü �ȿ� �ִ� �͵��� �����ؼ� vector<Vertex*>, vector<int>���·� ������ش�
		_parser->ConvertAll(e);

		vector<Vertex*> verticesInfo = e->m_opt_vertex;
		vector<int> indicesInfo = e->m_opt_index;

		for (auto&& i : verticesInfo)
		{
			VBData.emplace_back(MeshVertex{ i->m_pos, i->m_normal, XMFLOAT2{i->u, i->v} });
		}

		for (auto&& i : indicesInfo)
		{
			IBData.emplace_back(i + Offset);
		}

		Offset += e->m_opt_vertex.size();
	}

	delete _parser;
}