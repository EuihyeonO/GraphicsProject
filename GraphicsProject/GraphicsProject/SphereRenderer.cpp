#include "SphereRenderer.h"

SphereRenderer::SphereRenderer()
{
}

SphereRenderer::~SphereRenderer()
{
}

void SphereRenderer::Init()
{
    Renderer::Init();

    SetModelToSphere(20, 20);

    std::shared_ptr<RenderBase> Unit = *GetUnits().begin();
    Unit->SetTexture("skybox.dds");

    SetVSShader(L"EnvMapVertexShader.hlsl");
    SetPSShader(L"EnvMapPixelShader.hlsl");

    CreateConstantBuffer(EShaderType::VSShader, L"EnvMapVertexShader.hlsl", TransFormData);

    SetSampler("LINEARWRAP");

    SetPosition({ 4.0f, 0.0f, 5.0f });
}

void SphereRenderer::Update(float _DeltaTime)
{
}
