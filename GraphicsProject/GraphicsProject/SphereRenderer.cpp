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
    Unit->GetMeshData().TextureName = "skybox.dds";

    SetVSShader(L"EnvMapVertexShader.hlsl");
    SetPSShader(L"EnvMapPixelShader.hlsl");

    SetSampler("LINEARWRAP");
}

void SphereRenderer::Update(float _DeltaTime)
{
    TransFormData.WorldMatrix = DirectX::SimpleMath::Matrix::CreateScale(1.0f) * DirectX::SimpleMath::Matrix::CreateRotationY(0.0f) *
        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(4.0f, 0.0f, 5.0f));

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
