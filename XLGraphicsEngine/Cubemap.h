#pragma once

#include "MeshObject.h"

class Cubemap : public MeshObject
{
public:
	Cubemap(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~Cubemap();

private:
	virtual void BuildBuffers() override;
	virtual void SetTextures() override;
	virtual void BuildFX() override;
};

