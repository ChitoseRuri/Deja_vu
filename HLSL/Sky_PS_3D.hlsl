#include "Basic.hlsli"

// 像素着色器(3D)
float4 PS_Sky(VertexPosHWNormalTex pIn) : SV_Target
{	
    return g_Tex.Sample(g_SamLinear, pIn.Tex);
}