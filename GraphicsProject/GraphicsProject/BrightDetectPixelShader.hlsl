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
    
    float Luminance = 0.2126f * Color.r + 0.7152f * Color.g + 0.0722f * Color.b;
    
    if (Luminance > 0.8f)
    {
        return Color;
    }
    else
    {
        return (float4) 0.0f;
    }
}
