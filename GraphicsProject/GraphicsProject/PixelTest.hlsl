#include "LightHeader.hlsli"

cbuffer WorldLightBuffer : register(b0)
{
    float3 EyeWorld;
    float Padding;
    
    Light Lights[LIGHT_NUM];
};

cbuffer MaterialBuffer : register(b1)
{
    Material MaterialData;
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
    
    float3 LightSum = 0.0f;

    {
        float3 EyeDir = EyeWorld - _Input.WorldPos;
        
        float3 LightStrength = Lights[0].Strength * CalDirectionalLight(Lights[0], _Input.WorldNormal);
        LightSum += BlinnPhong(Lights[0], MaterialData, LightStrength, _Input.WorldNormal, EyeDir);
    }
    
    {
        float3 EyeDir = EyeWorld - _Input.WorldPos;
        
        float3 LightStrength = Lights[1].Strength * CalPointLight(Lights[1], _Input.WorldPos, _Input.WorldNormal);
        LightSum += BlinnPhong(Lights[1], MaterialData, LightStrength, _Input.WorldNormal, EyeDir);
    }
    
    {
        float3 EyeDir = EyeWorld - _Input.WorldPos;
        
        float3 LightStrength = Lights[2].Strength * CalSpotLight(Lights[2], _Input.WorldPos, _Input.WorldNormal);
        LightSum += BlinnPhong(Lights[2], MaterialData, LightStrength, _Input.WorldNormal, EyeDir);
    }


    Color.rgb *= LightSum;
    
    return Color;
}
