#include "BoxRenderer.h"

BoxRenderer::BoxRenderer()
{
    Name = "BOX";
}

BoxRenderer::~BoxRenderer()
{
}

void BoxRenderer::Render()
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
    
    int Index = 0;
    for (const ConstantBufferData& _Data : ConstantBuffers)
    {
        EngineBase::GetInstance().GetContext()->VSSetConstantBuffers(Index, 1, _Data.ConstantBuffer.GetAddressOf());
        EngineBase::GetInstance().GetContext()->PSSetConstantBuffers(Index, 1, _Data.ConstantBuffer.GetAddressOf());
        Index++;
    }
    
    UINT IndexCount = Indices.size();
    EngineBase::GetInstance().GetContext()->DrawIndexed(IndexCount, 0, 0);
}

void BoxRenderer::Init()
{
    CreateVertexAndIndex();
    
    RenderBase::CreateVertexBuffer();
    RenderBase::CreateIndexBuffer();
    RenderBase::CreateConstantBuffer<Transform>(TransFormData);
    RenderBase::CreateConstantBuffer(UV);
}

float Dt = 0.0f;

void BoxRenderer::Update()
{
    Dt += 0.01f;

    TransFormData.WorldMatrix = DirectX::SimpleMath::Matrix::CreateScale(0.5f) * DirectX::SimpleMath::Matrix::CreateRotationY(Dt) *
        DirectX::SimpleMath::Matrix::CreateTranslation(DirectX::SimpleMath::Vector3(0.0f, -0.3f, 1.0f));
    
    TransFormData.WorldMatrix = TransFormData.WorldMatrix.Transpose();
   
    TransFormData.ViewMAtrix =
        DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f });
   
    TransFormData.ViewMAtrix = TransFormData.ViewMAtrix.Transpose();

    const float AspectRatio = 1600.0f / 900.0f;

    const float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;
    TransFormData.ProjMatrix =
        DirectX::XMMatrixPerspectiveFovLH(fovAngleY, AspectRatio, 0.01f, 100.0f);

    TransFormData.ProjMatrix = TransFormData.ProjMatrix.Transpose();

    UV.x = 0.5f;

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

    std::vector<DirectX::SimpleMath::Vector3> Colors;
    Colors.reserve(24);

    std::vector<DirectX::SimpleMath::Vector3> Normals;
    Normals.reserve(24);

    std::vector<DirectX::SimpleMath::Vector2> TexCoords;
    TexCoords.reserve(24);

    //À­¸é
    Positions.push_back({ -1.0f, 1.0f, -1.0f });
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    
    Colors.push_back({1.0f, 0.0f, 0.0f});
    Colors.push_back({1.0f, 0.0f, 0.0f});
    Colors.push_back({1.0f, 0.0f, 0.0f});
    Colors.push_back({1.0f, 0.0f, 0.0f});

    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({ 0.0f, 1.0f, 0.0f});

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //¾Æ·§¸é
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //¿ÞÂÊ
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ -1.0f, 1.0f, -1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //¿À¸¥ÂÊ
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //¾ÕÂÊ
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });

    TexCoords.push_back({ 0.0f, 1.0f });
    TexCoords.push_back({ 0.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 0.0f });
    TexCoords.push_back({ 1.0f, 1.0f });

    //µÞÂÊ
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

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
        NewVertex.Color = Colors[i];
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