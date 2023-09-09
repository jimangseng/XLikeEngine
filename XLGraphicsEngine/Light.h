#pragma once
#include "IRenderableObject.h"
class Light :
    public IRenderableObject
{
public:
	Light();
	virtual ~Light();

public:
	virtual void Initialize(XLD3D11Renderer* _renderer) override;
	virtual void Finalize() override;

	virtual void Update();
	virtual void Render() override;

private:
	virtual void BuildFX() override;

};

