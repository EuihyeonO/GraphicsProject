#include "LightHeader.hlsli"

cbuffer WorldLights : register(b0)
{
    float3 EyeWorld;
    float Ambient;
    
    Light Lights[LIGHT_NUM];
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 TexCoord : TEXCOORD;
    
    float3 WorldPos : POSITION1;
    float3 WorldNormal : NORMAL;
};

Texture2D DiffuseTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PixelShaderInput _Input) : SV_TARGET
{
    float4 Color = DiffuseTexture.Sample(Sampler, _Input.TexCoord);
    
    float LightSum = 0.0f;
    
    float DiffuseLight = CalDiffuseLight(Lights[0], _Input.WorldNormal);
    float SpecularLight = CalSpecular_Phong(Lights[0], _Input.WorldNormal, EyeWorld - _Input.WorldPos);
    float AmbientLight = Ambient;
    
    LightSum = DiffuseLight + SpecularLight + AmbientLight;
     
    Color.rgb *= LightSum;
    
    return Color;
}
