#pragma once

#include <vector>
#include "XLD3D11Renderer.h"
#include "IRenderableObject.h"
#include "Utils/D3DHeaders.h"

class CASEParser;

struct MeshVertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 uv;
};


class MeshObject : public IRenderableObject
{
public:
	MeshObject();
	MeshObject(ID3D11Device* _device, ID3D11DeviceContext* _context);
	virtual ~MeshObject();
	
	virtual void Initialize(XLD3D11Renderer* _renderer) override;
	virtual void Finalize() override;

	virtual void Render() override;

	virtual void ParseMesh(const char* ASEFilePath);

private:
	virtual void BuildFX() override;
	virtual void BuildInputLayout() override;

	virtual void BuildBuffers();

	virtual void SetTextures();


	ID3D11Buffer* CreateVertexBuffer(std::vector<MeshVertex> verticesData, UINT sizeInByte) ;
	ID3D11Buffer* CreateIndexBuffer(std::vector<int> indicesData, UINT sizeInByte) ;

protected:

	ObjectType m_eType;

	// 오류 검출을 위한 HResult
	HRESULT hr;

	// Device & Device Context
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	// FX Framework 관련
	ID3DX11Effect* m_pFX;
	ID3DX11EffectTechnique* m_pTech;
	
	// ASE Parser 
	CASEParser* m_pParsedInfo;

	// Input Layout
	ID3D11InputLayout* m_pInputLayout;

	// World View Proj Matrices
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
	XMMATRIX WVP;

	// VB, IB
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	UINT m_NumberofIndices;

	XLD3D11Renderer* m_pRenderer;

	ID3D11ShaderResourceView* m_pDiffuse;
	ID3D11ShaderResourceView* m_pCubeMap;
	

protected:
	std::vector<MeshVertex> VBData;
	std::vector<int> IBData;

};

