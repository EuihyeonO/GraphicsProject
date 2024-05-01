cbuffer WorldViewProjection : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

// Semantics
// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics

struct VertexShaderInput
{
    float3 pos : POSITION;
    float3 color : COLOR0;
    float3 normal : NORMAL;
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

PixelShaderInput main(VertexShaderInput _Input)
{
    PixelShaderInput Output;
    float4 Pos = float4(_Input.pos, 1.0f);
    
    Pos = mul(Pos, World);
    Pos = mul(Pos, View);
    Pos = mul(Pos, Projection);

    Output.pos = Pos;
    Output.color = _Input.color;
    
    return Output;
}
