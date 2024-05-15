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

    SetVSShader(L"VertexTest.hlsl");
    SetPSShader(L"PixelTest.hlsl");
    SetSampler("LINEARWRAP");

    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::DragFloat3("RimLightColor", &RimLightData.RimColor.x, 0.01f, 0.0f, 1.0f); });
    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::DragFloat("RimPower", &RimLightData.RimPower, 0.01f, 0.0f, 5.0f); });
    EngineBase::GetInstance().AddGUIFunction([this] {ImGui::DragFloat("RimStrength", &RimLightData.RimStrength, 0.05f, 0.0f, 5.0f); });
}

void ZeldaRenderer::Update(float _DeltaTime)
{
    TransFormData.WorldMatrix = DirectX::SimpleMath::Matrix::CreateScale(0.02f) * DirectX::SimpleMath::Matrix::CreateRotationY(0.0f) *
        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -2.0f, 3.0f));

    TransFormData.WorldMatrix = TransFormData.WorldMatrix.Transpose();

    TransFormData.ViewMAtrix = EngineBase::GetInstance().ViewMat;
    TransFormData.ViewMAtrix = TransFormData.ViewMAtrix.Transpose();

    const float AspectRatio = 1600.0f / 900.0f;

    const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
    TransFormData.ProjMatrix =
        DirectX::XMMatrixPerspectiveFovLH(fovAngleY, AspectRatio, 0.01f, 100.0f);

    TransFormData.ProjMatrix = TransFormData.ProjMatrix.Transpose();

    TransFormData.InvTranspose = TransFormData.WorldMatrix;
    TransFormData.InvTranspose.Translation({ 0.0f, 0.0f, 0.0f });
    TransFormData.InvTranspose = TransFormData.InvTranspose.Transpose().Invert();
}
