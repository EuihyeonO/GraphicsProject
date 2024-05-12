#include "Renderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Init()
{
    isCallInit = true;

    CreateConstantBuffer(EShaderType::PSShader, L"PixelTest.hlsl", EngineBase::GetInstance().GetWorldLight());
    CreateConstantBuffer(EShaderType::PSShader, L"PixelTest.hlsl", MaterialData);
    CreateConstantBuffer(EShaderType::VSShader, L"VertexTest.hlsl", TransFormData);
}

void Renderer::Update(float _DeltaTime)
{
    TransFormData.WorldMatrix = DirectX::SimpleMath::Matrix::CreateScale(0.5f) * DirectX::SimpleMath::Matrix::CreateRotationY(0.0f) *
        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, 0.0f, 1.0f));

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

    for (const std::pair<std::wstring, std::list<EConstantBufferData>>& _DataPair : VSConstantBuffers)
    {
        const std::list<EConstantBufferData>& DataList = _DataPair.second;

        for (const EConstantBufferData& _Data : DataList)
        {
            D3D11_MAPPED_SUBRESOURCE Ms;

            EngineBase::GetInstance().GetContext()->Map(_Data.ConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &Ms);
            memcpy(Ms.pData, _Data.Data, _Data.DataSize);
            EngineBase::GetInstance().GetContext()->Unmap(_Data.ConstantBuffer.Get(), NULL);
        }
    }

    for (const std::pair<std::wstring, std::list<EConstantBufferData>>& _DataPair : PSConstantBuffers)
    {
        const std::list<EConstantBufferData>& DataList = _DataPair.second;

        for (const EConstantBufferData& _Data : DataList)
        {
            D3D11_MAPPED_SUBRESOURCE Ms;

            EngineBase::GetInstance().GetContext()->Map(_Data.ConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &Ms);
            memcpy(Ms.pData, _Data.Data, _Data.DataSize);
            EngineBase::GetInstance().GetContext()->Unmap(_Data.ConstantBuffer.Get(), NULL);
        }
    }
}

void Renderer::Render(float _DeltaTime)
{
	for (const std::shared_ptr<RenderBase> _RenderUnit : RenderUnits)
	{
		_RenderUnit->RenderSetting();
        SetConstantBuffer(_RenderUnit->GetVSShaderName(), _RenderUnit->GetPSShaderName());
        _RenderUnit->Render(_DeltaTime);
	}
}

void Renderer::SetConstantBuffer(const std::wstring& _VSShaderName, const std::wstring& _PSShaderName)
{
    int Index = 0;
    for (const EConstantBufferData& _Data : VSConstantBuffers[_VSShaderName])
    {
        EngineBase::GetInstance().GetContext()->VSSetConstantBuffers(Index, 1, _Data.ConstantBuffer.GetAddressOf());
        Index++;
    }

    Index = 0;
    for (const EConstantBufferData& _Data : PSConstantBuffers[_PSShaderName])
    {
        EngineBase::GetInstance().GetContext()->PSSetConstantBuffers(Index, 1, _Data.ConstantBuffer.GetAddressOf());
        Index++;
    }
}
