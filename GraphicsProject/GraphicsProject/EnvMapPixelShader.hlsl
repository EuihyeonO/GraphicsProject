#include "LightHeader.hlsli"

cbuffer WorldLightBuffer : register(b0)
{
    float3 EyeWorld;
    float Padding;
    
    Light Lights[LIGHT_NUM];
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    
    float3 WorldPos : POSITION1;
    float3 WorldNormal : NORMAL;
};

TextureCube CubeMapTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelShaderInput _Input) : SV_TARGET
{
    float3 EyeDir = _Input.WorldPos - EyeWorld;
    EyeDir = normalize(EyeDir);
    
    _Input.WorldNormal = normalize(_Input.WorldNormal);
    
    float3 RefEyeWorld = reflect(EyeDir, _Input.WorldNormal);
    
    float4 Color = CubeMapTexture.Sample(Sampler, RefEyeWorld);
    return Color;
}
