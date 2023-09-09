cbuffer cbPerObject
{
    float4x4 gWorldViewProj;
};

cbuffer cbPerFrame
{
    float3 gEyePosition;
    float3 gDirectionalLightDir;
    float3 gPointLightPosition;
    float3 gSpotLightPosition;
};

Texture2D gDiffuse;
Texture2D gDSB;
TextureCube gCubeMap;

SamplerState SolidState
{
    Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    
    float3 LightDirection;
};

struct PointLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    
    float3 Position;
    float3 LightDirection;
};

struct SpotLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    
    float3 Position;
    float3 LightDirection;
    float3 SpotDirection;
    float3 Target;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
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
    // Diffuse Map
    float4 TexColor = gDiffuse.Sample(SolidState, pin.UV);
    
    // Normal
    float3 SurfaceNormal = normalize(pin.Normal);

    // Output Variable
    float4 output = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Setting Material
    Material mat;
    mat.Ambient = float4(0.8f, 0.8f, 0.8f, 1.0f);
    mat.Diffuse = float4(1.0f, 1.0f, 1.0f, 1.0f); // w value of Diffuse is alpha.
    mat.Specular = float4(1.0f, 1.0f, 1.0f, 1.0f); // w value of Specular is Shininess.

    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    // Cubemapping
    // eye에서 position까지의 벡터를 구하고 그것의 반사벡터를 구한 뒤 둘을 더한다(?)
    
    // Cube Map
    
    //output = gCubeMap.Sample(SolidState, pin.PosH);
    
    float3 view4 = (pin.PosW - gEyePosition);
    float3 reflect4 = reflect(view4, SurfaceNormal);
    
    float3 cube = view4 + reflect4;
    float4 CubeMapColor = gCubeMap.Sample(SolidState, cube);
    
    
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Directional Light

    // Setting Directional Light
    DirectionalLight DLight1;
    DLight1.LightDirection = normalize(gDirectionalLightDir);
    DLight1.Ambient = float4(0.3f, 0.3f, 0.3f, 1.0f);
    DLight1.Diffuse = float4(0.4f, 0.4f, 0.4f, 1.0f);
    DLight1.Specular = float4(0.5f, 0.5f, 0.5f, 1.0f);

    // Compute Ambient Term
    float4 AmbientTerm = DLight1.Ambient * mat.Ambient;

    // Compute Diffuse Term
    float4 DiffuseTerm = dot(SurfaceNormal, -DLight1.LightDirection) * (DLight1.Diffuse * mat.Diffuse);
   
    // Compute Specular Specular
    float4 SpecularTerm = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float3 Reflect = normalize(reflect(DLight1.LightDirection, SurfaceNormal));
    float3 View = normalize(gEyePosition - pin.PosW);
    
    if (dot(-DLight1.LightDirection, SurfaceNormal) > 0) // 빛을 받는 방향에서만 specular 적용
    {
        SpecularTerm = pow(max(dot(Reflect, View), 0), 8) * (DLight1.Specular * mat.Specular);
    }
    
	output += (TexColor * CubeMapColor) * (AmbientTerm + DiffuseTerm + SpecularTerm);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Point Light
    
    // Setting Point Light
    PointLight PLight1;
    PLight1.Position = gPointLightPosition;
    PLight1.LightDirection = normalize(pin.PosW - PLight1.Position);
    PLight1.Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    PLight1.Diffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
    PLight1.Specular = float4(0.7f, 0.7f, 0.7f, 1.0f);
    
    // Compute Ambient Term
    float4 AmbientTerm2 = PLight1.Ambient * mat.Ambient;

    // Compute Diffuse Term
    float4 DiffuseTerm2 = dot(SurfaceNormal, -PLight1.LightDirection) * (PLight1.Diffuse * mat.Ambient);

    // Compute Specular Term
    float4 SpecularTerm2 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float3 Reflect2 = normalize(reflect(PLight1.LightDirection, SurfaceNormal));
    float3 View2 = normalize(gEyePosition - pin.PosW);
    
    if (dot(-PLight1.LightDirection, SurfaceNormal) > 0) // 빛을 받는 방향에서만 specular 적용
    {
        SpecularTerm2 = pow(max(dot(Reflect2, View2), 0), 16) * float4(0.7f, 0.7f, 0.7f, 1.0f);
    }
    
    // Attanuation
    float DistancePow = pow((PLight1.Position.x - pin.PosW.x), 2) + pow((PLight1.Position.y - pin.PosW.y), 2) + pow((PLight1.Position.z - pin.PosW.z), 2);
    float Distance = sqrt(DistancePow);
    float Attanuation = 0.0f + 0.0f * Distance + 1.0f * DistancePow;
    
	output += (TexColor * CubeMapColor) * (AmbientTerm2 + (DiffuseTerm2 + SpecularTerm2) / Attanuation);
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Spot Light
    
    // Setting Spot Light
    SpotLight SLight1;
    SLight1.Position = gSpotLightPosition;
    SLight1.LightDirection = normalize(pin.PosW - SLight1.Position); // SpotLight의 위치에서 오브젝트의 한 점으로 향하는 벡터
    SLight1.Target = float3(0.0f, 1.0f, 0.0f);
    SLight1.SpotDirection = normalize( SLight1.Target - SLight1.Position); // SpotLight의 위치에서 Target 으로 향하는 벡터
    SLight1.Ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    SLight1.Diffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
    SLight1.Specular = float4(0.8f, 0.8f, 0.8f, 1.0f);
    
    
    // Compute Ambient Term
    float4 AmbientTerm3 = SLight1.Ambient * mat.Ambient;
    
    // Compute Diffuse Term
    float4 DiffuseTerm3 = dot(SurfaceNormal, -SLight1.LightDirection) * (SLight1.Diffuse * mat.Diffuse);
    
    // Compute Specular Term
    float4 SpecularTerm3 = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    float3 Reflect3 = normalize(reflect(SLight1.LightDirection, SurfaceNormal));
    float3 View3 = normalize(gEyePosition - pin.PosW);
    
    if (dot(-SLight1.LightDirection, SurfaceNormal) > 0) // 빛을 받는 방향에서만 specular 적용
    {
        SpecularTerm3 = pow(max(dot(Reflect3, View3), 0), 8) * (SLight1.Specular * mat.Specular);
    }
        
    // Compute Attanuation
    float DistancePow2 = pow((SLight1.Position.x - pin.PosW.x), 2) + pow((SLight1.Position.y - pin.PosW.y), 2) + pow((SLight1.Position.z - pin.PosW.z), 2);
    float Distance2 = sqrt(DistancePow2);
    float Attanuation2 = 0.0f + 0.0f * Distance2 + 1.0f * DistancePow2;
    
    // Compute Spot Coefficient
    float SpotCoefficient = pow(dot(SLight1.LightDirection, SLight1.SpotDirection), 128); // dot(-L, d) == cos x
    
	output += ((TexColor * CubeMapColor) * (AmbientTerm3 + (DiffuseTerm3 + SpecularTerm3) / Attanuation2)) * SpotCoefficient;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    

    
    
    
    
    //output = gDSB.Sample(SolidState, pin.UV);
    
    
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
