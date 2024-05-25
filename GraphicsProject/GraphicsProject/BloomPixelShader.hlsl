#include "LightHeader.hlsli"

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
};

cbuffer EBloomData : register(b0)
{
    int Width;
    int Height;
    int Padding1;
    int Padding2;
};

Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

static float Gau[5][5] =
{
    { 1, 4, 6, 4, 1 },
    { 4, 16, 24, 16, 4 },
    { 6, 24, 36, 24, 6 },
    { 4, 16, 24, 16, 4 },
    { 1, 4, 6, 4, 1 }
};

float4 main(PixelShaderInput _Input) : SV_TARGET
{
    float4 Color = (float4)0.0f;
    
    float WidthRatio = 1.0f / (float) Width;
    float HeightRatio = 1.0f / (float) Height;
    
    float2 StartTexCoord = float2(_Input.TexCoord.x - WidthRatio * 2, _Input.TexCoord.y - HeightRatio * 2);
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            StartTexCoord.y += HeightRatio;
            Color += DiffuseTexture.Sample(Sampler, StartTexCoord.xy) * Gau[j][i];
        }
        
        StartTexCoord.x += WidthRatio;
        StartTexCoord.y = _Input.TexCoord.y;
    }

    Color /= 256.0f;
    
    return Color;
}
