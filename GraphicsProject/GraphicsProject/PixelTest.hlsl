struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

float4 main(PixelShaderInput _Input) : SV_TARGET
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
