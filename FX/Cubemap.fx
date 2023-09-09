cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};

TextureCube gCubeMap;
Texture2D gDSB;

SamplerState SolidState
{
    Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float4 PosH : SV_POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
	// Transform to Homogeneous clip space. 
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	
    vout.PosW = vin.PosL;
    vout.Normal = vin.Normal;
    vout.UV = vin.UV;
    
    return vout;
}

float4 PS(VertexOut pin) : SV_Target0
{
    float4 output = gCubeMap.Sample(SolidState, pin.PosW);
    // 솔직히 왜 포지션을 샘플링하면 되는지 이해가 잘 안간다..
    
    return output;
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
		
		SetBlendState(NULL, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);

	}
}