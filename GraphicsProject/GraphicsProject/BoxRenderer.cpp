#include "BoxRenderer.h"

BoxRenderer::BoxRenderer()
{
    Name = "BOX";
}

BoxRenderer::~BoxRenderer()
{
}

void BoxRenderer::Init()
{
    RenderBase::Init();

    CreateVertexAndIndex();
    
    RenderBase::CreateVertexBuffer();
    RenderBase::CreateIndexBuffer();

    SetVSShader(L"VertexTest.hlsl");
    SetPSShader(L"PixelTest.hlsl");

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

void BoxRenderer::CreateVertexAndIndex()
{
    std::vector<DirectX::SimpleMath::Vector3> Positions;
    Positions.reserve(24);

    std::vector<DirectX::SimpleMath::Vector3> Normals;
    Normals.reserve(24);

    std::vector<DirectX::SimpleMath::Vector2> TexCoords;
    TexCoords.reserve(24);

    //À­¸é
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

    //¾Æ·§¸é
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

    //¿ÞÂÊ
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

    //¿À¸¥ÂÊ
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

    //¾ÕÂÊ
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

    //µÞÂÊ
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
        EVertex NewVertex;
        NewVertex.Position = Positions[i];
        NewVertex.Normal = Normals[i];
        NewVertex.TexCoord = TexCoords[i];

        MeshData.Vertices.push_back(NewVertex);
    }

    MeshData.Indices = { 0, 1, 2,
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