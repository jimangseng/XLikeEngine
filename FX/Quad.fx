cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
};


struct VertexIn
{
	float2 PosH : POSITION;
	float2 UV : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D gDiffuse;

SamplerState SolidState
{
	Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
    
	vout.PosH = float4(vin.PosH, 0.0f, 1.0f);
	vout.UV = vin.UV;
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target0
{
	float4 pout = gDiffuse.Sample(SolidState, pin.UV);
	
	//float4 pout = float4(1.0f, 1.0f, 0.0f, 1.0f);
	
	return pout;
}


technique11 DeferredTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}