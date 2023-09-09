#pragma once

#include <dxgi1_2.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "../Utils/d3dx11effect.h"



#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#ifdef _DEBUG
	#pragma comment(lib, "Effects11_debug.lib")
#else
	#pragma comment(lib, "Effects11.lib")
#endif


enum class ObjectType
{
	StaticMesh,
	CubeMap
};

using float2 = DirectX::XMFLOAT2;
using float3 = DirectX::XMFLOAT3;
using float4 = DirectX::XMFLOAT4;
using matrix = DirectX::XMMATRIX;