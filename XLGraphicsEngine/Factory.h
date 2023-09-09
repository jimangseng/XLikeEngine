#pragma once

#include "Utils/D3DHeaders.h"

class IRenderableObject;
class MeshObject;

class Factory
{
public:
	Factory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~Factory();

	MeshObject* CreateStaticMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	IRenderableObject* CreateSkinnedMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	IRenderableObject* CreateHelperObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
};

