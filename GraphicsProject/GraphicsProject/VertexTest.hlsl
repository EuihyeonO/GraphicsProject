#include "LightHeader.hlsli"

cbuffer TransformData : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix InvTranspose;
};

struct VertexShaderInput
{
    float3 pos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    
    float3 WorldPos : POSITION1;
    float3 WorldNormal : NORMAL;
};

PixelShaderInput main(VertexShaderInput _Input)
{
    PixelShaderInput Output;
    float4 Pos = float4(_Input.pos, 1.0f);
    
    Pos = mul(Pos, World);
    Pos = mul(Pos, View);
    Pos = mul(Pos, Projection);

    Output.pos = Pos;
    Output.TexCoord = _Input.TexCoord;
    
    Output.WorldPos = mul(float4(_Input.pos, 1.0f), World).rgb;
    Output.WorldNormal = mul(float4(_Input.Normal, 0.0f), InvTranspose).rgb;
    
    return Output;
}
