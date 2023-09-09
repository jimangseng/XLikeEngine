#include "Factory.h"

#include "Objects.h"

#include "Utils/D3DHeaders.h"

Factory::Factory(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:m_pDevice(pDevice), m_pDeviceContext(pDeviceContext)
{

}

Factory::~Factory()
{

}

MeshObject* Factory::CreateStaticMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	return new MeshObject(m_pDevice, m_pDeviceContext);
}

IRenderableObject* Factory::CreateSkinnedMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	//return new SkinnedMesh(m_pDevice, m_pDeviceContext);
	return nullptr;
}

IRenderableObject* Factory::CreateHelperObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	//return new HelperObject(m_pDevice, m_pDeviceContext);
	return nullptr;
}
