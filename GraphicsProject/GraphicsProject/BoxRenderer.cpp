#include "BoxRenderer.h"

BoxRenderer::BoxRenderer()
{
    Name = "BOX";
}

BoxRenderer::~BoxRenderer()
{
}

void BoxRenderer::Render(float _DeltaTime)
{
    UINT Stride = sizeof(Vertex);
    UINT Offset = 0;
    
    VertexShaderData VSData = EngineBase::GetInstance().GetVertexShaderData(L"VertexTest.hlsl");
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PS = EngineBase::GetInstance().GetPixelShaderData(L"PixelTest.hlsl");

    EngineBase::GetInstance().GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);
    EngineBase::GetInstance().GetContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    EngineBase::GetInstance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    EngineBase::GetInstance().GetContext()->VSSetShader(VSData.VertexShader.Get(), 0, 0);
    EngineBase::GetInstance().GetContext()->IASetInputLayout(VSData.InputLayout.Get());
    EngineBase::GetInstance().GetContext()->PSSetShader(PS.Get(), 0, 0);

    //추후 텍스쳐 여러개 세팅할 수도 있다.
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV = EngineBase::GetInstance().GetTextureData(TextureName).ShaderResourceView;
    Microsoft::WRL::ComPtr<ID3D11SamplerState> Sampler = EngineBase::GetInstance().GetSampler(SamplerName);
    
    EngineBase::GetInstance().GetContext()->PSSetShaderResources(0, 1, SRV.GetAddressOf());
    EngineBase::GetInstance().GetContext()->PSSetSamplers(0, 1, Sampler.GetAddressOf());

    int Index = 0;
    for (const ConstantBufferData& _Data : ConstantBuffers)
    {
        EngineBase::GetInstance().GetContext()->VSSetConstantBuffers(Index, 1, _Data.ConstantBuffer.GetAddressOf());
        EngineBase::GetInstance().GetContext()->PSSetConstantBuffers(Index, 1, _Data.ConstantBuffer.GetAddressOf());
        Index++;
    }
    
    UINT IndexCount = (UINT)Indices.size();
    EngineBase::GetInstance().GetContext()->DrawIndexed(IndexCount, 0, 0);
}

void BoxRenderer::Init()
{
    RenderBase::Init();

    CreateVertexAndIndex();
    
    RenderBase::CreateVertexBuffer();
    RenderBase::CreateIndexBuffer();

    SetTexture("BoxTexture.png");
    SetSampler("LINEARWRAP");
}

void BoxRenderer::Update(float _DeltaTime)
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

    for (const ConstantBufferData& _Data : ConstantBuffers)
    {
        D3D11_MAPPED_SUBRESOURCE Ms;
        
        EngineBase::GetInstance().GetContext()->Map(_Data.ConstantBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &Ms);
        memcpy(Ms.pData, _Data.Data, _Data.DataSize);
        EngineBase::GetInstance().GetContext()->Unmap(_Data.ConstantBuffer.Get(), NULL);
    }
}

void BoxRenderer::CreateVertexAndIndex()
{
    std::vector<DirectX::SimpleMath::Vector3> Positions;
    Positions.reserve(24);

    std::vector<DirectX::SimpleMath::Vector3> Normals;
    Normals.reserve(24);

    std::vector<DirectX::SimpleMath::Vector2> TexCoords;
    TexCoords.reserve(24);

    //윗면
    Positions.push_back({ -1.0f, 1.0f, -1.0f });
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({ 0.0f, 1.0f, 0.0f});

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //아랫면
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });

    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //왼쪽
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ -1.0f, 1.0f, -1.0f });

    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //오른쪽
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });

    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //앞쪽
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });

    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //뒷쪽
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });

    Normals.push_back({ 0.0f, 0.0f, -1.0f });
    Normals.push_back({ 0.0f, 0.0f, -1.0f });
    Normals.push_back({ 0.0f, 0.0f, -1.0f });
    Normals.push_back({ 0.0f, 0.0f, -1.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    for (size_t i = 0; i < 24; i++) 
    {
        Vertex NewVertex;
        NewVertex.Position = Positions[i];
        NewVertex.Normal = Normals[i];
        NewVertex.TexCoord = TexCoords[i];

        Vertices.push_back(NewVertex);
    }

    Indices = { 0, 1, 2,
                0, 2, 3,
                4, 6, 5,
                4, 7, 6,
                8, 9, 10,
                8, 10, 11,
                12, 13, 14,
                12, 14, 15,
                16, 17, 18,
                16, 18, 19,
                20, 21, 22,
                20, 22, 23 };
}