#pragma once

#include "Utils/D3DHeaders.h"
#include <DirectXMath.h>


#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

using namespace DirectX;

/// <summary>
/// ��å ������ ��� ������Ʈ�� ����°��� �����ֱ� ���� ����
/// 6���� Box������ �����Ѵ�.
///
/// �������� �ּ��� �� �޾ƺô�.
/// 
/// 2021.03.24 LeHideKGIT
/// </summary>
class Box
{
public:
	Box(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11RasterizerState* pRS);
	~Box();

	struct Vertex
	{
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

public:
	void Initialize();	// �ʱ�ȭ
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);		// ����� ������Ʈ �� ��ȯ��ĸ� �����Ѵ�.
	void Render();		// �׸���. ������ BeginRender()�� EndRender()���̿� ȣ�� �� ���� ����Ѵ�.


private:
	void BuildGeometryBuffers();		// ���ϱ����κ��� ���ؽ�/�ε������۸� �����.
	void BuildFX();						// ����Ʈ�� �����. (���̴��� ����ϱ� ���ؼ�)
	void BuildVertexLayout();			// ���ؽ����̾ƿ��� �����.


	/// �ڽ��� �׸��� ���ؼ��� ���� �־�� �ϳ�..
	// �Ϻδ� �����͸� (�����ּ�)
	// �Ϻδ� ���Ⱑ ���� (������ �ڿ� ������ ���� ���Ⱑ �ƴ� ���� �ִ�)
private:
	ID3D11Device* md3dDevice;						/// D3D11 ����̽�
	ID3D11DeviceContext* md3dImmediateContext;		/// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* mVB;	// ���ؽ�����
	ID3D11Buffer* mIB;	// �ε�������

	ID3DX11Effect* mFX;								// ����Ʈ
	ID3DX11EffectTechnique* mTech;					// ��ũ
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;	// ����Ʈ�뺯��

	ID3D11InputLayout* mInputLayout;				// ��ǲ���̾ƿ�

	// ��ȯ ����
	XMFLOAT4X4 mWorld;	// ���� ��ȯ ��� (����->����)
	XMFLOAT4X4 mView;	// �þ� ��ȯ ��� (ī�޶� ��)
	XMFLOAT4X4 mProj;	// ���� ��ȯ ��� (����/����)


	// ����������Ʈ. �������� ��� �� ���ΰ��� ���� ��.
	ID3D11RasterizerState* m_pRenderstate;			/// �ܺο��� �����ؼ� ������ ��Ȳ�� ���� ������. ���̴����� �ص� �ȴ�.
};

