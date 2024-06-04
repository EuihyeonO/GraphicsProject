#include "ZeldaRenderer.h"

ZeldaRenderer::ZeldaRenderer()
{
}

ZeldaRenderer::~ZeldaRenderer()
{
}

void ZeldaRenderer::Init()
{
    Renderer::Init();

    SetModel("zeldaPosed001.fbx");

    SetVSShader(L"MeshLightVertexShader.hlsl");
    SetPSShader(L"MeshLightPixelShader.hlsl");
    SetSampler("LINEARWRAP");

    CreateConstantBuffer(EShaderType::VSShader, L"MeshLightVertexShader.hlsl", TransFormData);

    CreateConstantBuffer(EShaderType::PSShader, L"MeshLightPixelShader.hlsl", EngineBase::GetInstance().GetWorldLight());
    CreateConstantBuffer(EShaderType::PSShader, L"MeshLightPixelShader.hlsl", MaterialData);
    CreateConstantBuffer(EShaderType::PSShader, L"MeshLightPixelShader.hlsl", RimLightData);

    SetScale({ 0.02f, 0.02f, 0.02f });
    SetPosition({ 0.0f, -2.0f, 3.0f });

    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::Text("Zelda"); ImGui::Separator(); });
    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::SliderFloat3("RimColor", &RimLightData.RimColor.x, 0.0f, 1.0f); });
    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::SliderFloat("RimPower", &RimLightData.RimPower, 0.0f, 5.0f); });
    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::SliderFloat("RimStrength", &RimLightData.RimStrength, 0.0f, 5.0f); });
}

void ZeldaRenderer::Update(float _DeltaTime)
{
}
