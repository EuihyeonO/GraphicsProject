#include "BoxRenderer.h"

BoxRenderer::BoxRenderer()
{
}

BoxRenderer::~BoxRenderer()
{
}

void BoxRenderer::Init()
{
    Renderer::Init();
    
    SetModelToCube("skybox.dds");
    
    SetVSShader(L"CubeMapVertexShader.hlsl");
    SetPSShader(L"CubeMapPixelShader.hlsl");

    CreateConstantBuffer(EShaderType::VSShader, L"CubeMapPixelShader.hlsl", EngineBase::GetInstance().GetWorldLight());
    CreateConstantBuffer(EShaderType::VSShader, L"CubeMapVertexShader.hlsl", TransFormData);

    SetSampler("LINEARWRAP");
    SetScale({ 10.0f, 10.0f, 10.0f });
}

void BoxRenderer::Update(float _DeltaTime)
{
}