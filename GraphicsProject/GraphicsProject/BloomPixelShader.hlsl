#include "LightHeader.hlsli"

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelShaderInput _Input) : SV_TARGET
{
    float4 Color = DiffuseTexture.Sample(Sampler, _Input.TexCoord);
    
    return Color;
}
