cbuffer UV : register(b1)
{
    float4 UV;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
    float2 TexCoord : TEXCOORD;
};

float4 main(PixelShaderInput _Input) : SV_TARGET
{
    if(UV.x > _Input.TexCoord.x)
    {
        return float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        return float4(0.0f, 0.0f, 1.0f, 1.0f);
    }
}
