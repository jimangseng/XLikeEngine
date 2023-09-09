#pragma once

#include "Utils/D3DHeaders.h"
#include <DirectXMath.h>


#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

using namespace DirectX;

/// <summary>
/// 용책 예제를 어떻게 오브젝트로 만드는가를 보여주기 위한 예제
/// 6장의 Box예제에 대응한다.
///
/// 오랜만에 주석을 좀 달아봤다.
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
	void Initialize();	// 초기화
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);		// 현재는 업데이트 시 변환행렬만 갱신한다.
	void Render();		// 그린다. 엔진의 BeginRender()와 EndRender()사이에 호출 될 것을 기대한다.


private:
	void BuildGeometryBuffers();		// 기하구조로부터 버텍스/인덱스버퍼를 만든다.
	void BuildFX();						// 이펙트를 만든다. (쉐이더를 사용하기 위해서)
	void BuildVertexLayout();			// 버텍스레이아웃을 만든다.


	/// 박스를 그리기 위해서는 뭐가 있어야 하나..
	// 일부는 포인터만 (세줄주석)
	// 일부는 여기가 원본 (하지만 자원 관리에 따라서 여기가 아닐 수도 있다)
private:
	ID3D11Device* md3dDevice;						/// D3D11 디바이스
	ID3D11DeviceContext* md3dImmediateContext;		/// 디바이스 컨텍스트

	ID3D11Buffer* mVB;	// 버텍스버퍼
	ID3D11Buffer* mIB;	// 인덱스버퍼

	ID3DX11Effect* mFX;								// 이펙트
	ID3DX11EffectTechnique* mTech;					// 테크
	ID3DX11EffectMatrixVariable* mfxWorldViewProj;	// 이펙트용변수

	ID3D11InputLayout* mInputLayout;				// 인풋레이아웃

	// 변환 관련
	XMFLOAT4X4 mWorld;	// 월드 변환 행렬 (로컬->월드)
	XMFLOAT4X4 mView;	// 시야 변환 행렬 (카메라 뷰)
	XMFLOAT4X4 mProj;	// 투영 변환 행렬 (원근/직교)


	// 렌더스테이트. 렌더링을 어떻게 할 것인가에 대한 것.
	ID3D11RasterizerState* m_pRenderstate;			/// 외부에서 생성해서 적당히 상황에 따라서 적용함. 쉐이더에서 해도 된다.
};

