#pragma once

#include "Utils/D3DHeaders.h"
#include "XLASEParser/CParsingDataClass.h"


class XLD3D11Renderer;
class CASEParser;
class Mesh;

class IRenderableObject
{

public:
	IRenderableObject();
	virtual ~IRenderableObject();

	virtual void Initialize(XLD3D11Renderer* _renderer) abstract;
	virtual void Finalize() abstract;

	virtual void Render() abstract;

private:
	virtual void BuildFX() abstract;
	virtual void BuildInputLayout() abstract;


};

