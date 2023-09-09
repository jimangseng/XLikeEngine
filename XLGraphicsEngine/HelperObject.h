#pragma once

#include "IRenderableObject.h"

class HelperObject : public IRenderableObject
{
public:
	HelperObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~HelperObject();

	virtual void Initialize(XLD3D11Renderer* _renderer) override;
	virtual void Finalize() override;

	virtual void Render() override;

private:
	virtual void BuildFX() override;
};

