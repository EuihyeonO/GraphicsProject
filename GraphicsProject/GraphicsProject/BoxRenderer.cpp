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

    SetSampler("LINEARWRAP");
}

void BoxRenderer::Update(float _DeltaTime)
{
    TransFormData.WorldMatrix = DirectX::SimpleMath::Matrix::CreateScale(10.0f) * DirectX::SimpleMath::Matrix::CreateRotationY(0.0f) *
        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f));

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

void BoxRenderer::CreateVertexAndIndex()
{

}