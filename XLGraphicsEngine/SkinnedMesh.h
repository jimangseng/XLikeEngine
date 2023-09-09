#pragma once

#include "IRenderableObject.h"

class SkinnedMesh : public IRenderableObject
{
public:
	SkinnedMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	~SkinnedMesh();

	virtual void Initialize(XLD3D11Renderer* _renderer) override;
	virtual void Finalize() override;

	virtual void Render() override;

private:
	virtual void BuildFX() override;
};

