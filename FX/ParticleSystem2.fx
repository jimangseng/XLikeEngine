//struct Particle
//{
//    float3 InitialPosW : POSITION0;
//    float3 InitialVelW : VELOCITY;
//    float2 SizeW : SIZE;
//    float Age : AGE;
//    uint Type : TYPE;
//};

//DepthStencilState DisableDepth
//{
//    DepthEnable = false;
//    DepthWriteMask = ZERO;
//};

//Particle StreamOutVS(Particle vin)
//{
//    return vin;
//}

//void StreamOutGS(point Particle gin[1], inout PointStream<Particle> ptStream)
//{
    
//}

//GeometryShader gsStreamOut = ConstructGSWithSO(
//    CompileShader(gs_5_0, StreamOutGS()),
//    "POSITION.xyz; VELOCITY.xyz; SIZE.xy; AGE.x TYPE.x"
//);

//technique11 StreamOutputTech
//{
//    pass P0
//    {
//        SetVertexShader(CompileShader(vs_5_0, StreamOutVS()));
//        SetGeometryShader(gsStreamOut);
//        SetPixelShader(NULL);
//        SetDepthStencilState(DisableDepth, 0);
//    }
//}