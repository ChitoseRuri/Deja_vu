#include "Basic.hlsli"

VertexPosHWNormalTex VS_Sky(VertexPosNormalTex vIn)
{
    VertexPosHWNormalTex vOut;
    matrix viewProj = mul(g_View, g_Proj);
    float4 posW = mul(float4(vIn.PosL, 1.0f), g_World);

    vOut.PosH = mul(posW, viewProj).xyww;
    vOut.PosW = posW.xyz;
    vOut.Tex = vIn.Tex;
    return vOut;
}
