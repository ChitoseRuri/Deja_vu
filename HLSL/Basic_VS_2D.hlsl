#include "Basic.hlsli"

// 顶点着色器(2D)
VertexPosHTex VS_2D(VertexPosTex vIn)
{
    VertexPosHTex vOut;
    vOut.PosH = mul(float4(vIn.PosL, 1.0f), g_World);
    vOut.Tex = vIn.Tex;
    return vOut;
}