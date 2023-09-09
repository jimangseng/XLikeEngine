#include "GBuffer.h"

#include "Quad.h"

static HRESULT hr = NULL;

GBuffer::GBuffer(ID3D11Device* device, ID3D11DeviceContext* dc)
	:m_pDevice(device), m_pDeviceContext(dc)
{
	CreateBuffer();
	CreateRenderTargetView(m_Buffer);
	CreateShaderResourceView(m_Buffer);
	m_Quad = new Quad(device, dc);
}

GBuffer::~GBuffer()
{

}

void GBuffer::CreateBuffer()
{
	D3D11_TEXTURE2D_DESC RT_d {};

	// description 구조체 채워넣을 것
	RT_d.Width = 1184;
	RT_d.Height = 861;
	RT_d.MipLevels = 0;
	RT_d.ArraySize = 1;
	RT_d.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	RT_d.SampleDesc = {1, 0};
	RT_d.Usage = D3D11_USAGE_DEFAULT;
	RT_d.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	RT_d.CPUAccessFlags = 0;
	RT_d.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA SubresourceData;
	//SubresourceData.pSysMem;

	hr = m_pDevice->CreateTexture2D(&RT_d, nullptr, &m_Buffer);
}

void GBuffer::CreateRenderTargetView(ID3D11Texture2D* Buffer)
{
	D3D11_RENDER_TARGET_VIEW_DESC RTV_d {};

	// description 구조체 채워넣을 것
	RTV_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	RTV_d.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	hr = m_pDevice->CreateRenderTargetView(m_Buffer, &RTV_d, &m_RTV);
}

void GBuffer::CreateShaderResourceView(ID3D11Texture2D* RenderTarget)
{
	D3D11_SHADER_RESOURCE_VIEW_DESC SRV_d {};

	SRV_d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRV_d.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D; //???
	SRV_d.Texture2D.MostDetailedMip = 0;
	SRV_d.Texture2D.MipLevels = 3;

	hr = m_pDevice->CreateShaderResourceView(m_Buffer, &SRV_d, &m_SRV);
}

ID3D11RenderTargetView* GBuffer::GetRTV()
{
	return m_RTV;
}

ID3D11ShaderResourceView* GBuffer::GetSRV()
{
	return m_SRV;
}
