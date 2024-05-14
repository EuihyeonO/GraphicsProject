
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
    float4 Color = CubeMapTexture.Sample(Sampler, _Input.WorldPos.xyz);
    return Color;
}
