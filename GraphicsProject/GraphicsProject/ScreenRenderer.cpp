#include "ScreenRenderer.h"

ScreenRenderer::ScreenRenderer()
{
}

ScreenRenderer::~ScreenRenderer()
{
}

void ScreenRenderer::Init()
{
    Renderer::Init();

    SetModelToSquare(1.0f);
    SetTransform();
}

void ScreenRenderer::Update(float _DeltaTime)
{

}

void ScreenRenderer::SetTransform()
{
    TransFormData.WorldMatrix = DirectX::SimpleMath::Matrix::CreateScale(1.0f) * DirectX::SimpleMath::Matrix::CreateRotationY(0.0f) *
        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

    TransFormData.WorldMatrix = TransFormData.WorldMatrix.Transpose();

    TransFormData.ViewMAtrix = EngineBase::GetInstance().ViewMat;
    TransFormData.ViewMAtrix = TransFormData.ViewMAtrix.Transpose();

    TransFormData.ProjMatrix =
        DirectX::XMMatrixOrthographicOffCenterLH(-1.0f, 1.0f, -1.0f, 1.0f,
            0.01f, 100.0f);

    TransFormData.ProjMatrix = TransFormData.ProjMatrix.Transpose();

    TransFormData.InvTranspose = TransFormData.WorldMatrix;
    TransFormData.InvTranspose.Translation({ 0.0f, 0.0f, 0.0f });
    TransFormData.InvTranspose = TransFormData.InvTranspose.Transpose().Invert();
}
