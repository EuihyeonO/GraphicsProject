#include "Renderer.h"
#include "ResourceManager.h"

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
    CreateConstantBuffer(EShaderType::PSShader, L"PixelTest.hlsl", RimLightData);

    CreateConstantBuffer(EShaderType::VSShader, L"CubeMapPixelShader.hlsl", EngineBase::GetInstance().GetWorldLight());

    CreateConstantBuffer(EShaderType::VSShader, L"VertexTest.hlsl", TransFormData);

    CreateConstantBuffer(EShaderType::VSShader, L"CubeMapVertexShader.hlsl", TransFormData);

    CreateConstantBuffer(EShaderType::VSShader, L"EnvMapVertexShader.hlsl", TransFormData);

}

void Renderer::Update(float _DeltaTime)
{
}

void Renderer::ConstantBufferUpdate()
{
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

void Renderer::SetVSShader(const std::wstring& _Shadername)
{
    for (std::shared_ptr<RenderBase> _RenderUnit : RenderUnits)
    {
        _RenderUnit->SetVSShader(_Shadername);
    }
}

void Renderer::SetPSShader(const std::wstring& _Shadername)
{
    for (std::shared_ptr<RenderBase> _RenderUnit : RenderUnits)
    {
        _RenderUnit->SetPSShader(_Shadername);
    }
}

void Renderer::SetSampler(const std::string& _Sampler)
{
    for (std::shared_ptr<RenderBase> _RenderUnit : RenderUnits)
    {
        _RenderUnit->SetSampler(_Sampler);
    }
}

void Renderer::SetModel(const std::string& _Name)
{
    const std::list<EMeshData>& MeshData = ResourceManager::GetLoadedMeshList(_Name);

    for (const EMeshData& Mesh : MeshData)
    {
        std::shared_ptr<RenderBase> NewRenderUnit = std::make_shared<RenderBase>();
        
        NewRenderUnit->SetMesh(Mesh);
        NewRenderUnit->CreateBuffer();

        RenderUnits.push_back(NewRenderUnit);
    }
}

void Renderer::SetModelToCube(const std::string& _CubeMapTextureName)
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

    Normals.push_back({ 0.0f, 1.0f, 0.0f });
    Normals.push_back({ 0.0f, 1.0f, 0.0f });
    Normals.push_back({ 0.0f, 1.0f, 0.0f });
    Normals.push_back({ 0.0f, 1.0f, 0.0f });

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

    std::shared_ptr<RenderBase> NewUnit = std::make_shared<RenderBase>();

    for (size_t i = 0; i < 24; i++)
    {
        EVertex NewVertex;
        NewVertex.Position = Positions[i];
        NewVertex.Normal = Normals[i];
        NewVertex.TexCoord = TexCoords[i];

        NewUnit->GetMeshData().Vertices.push_back(NewVertex);
    }

    NewUnit->GetMeshData().Indices = { 2, 1, 0,
                3, 2, 0,
                5, 6, 4,
                6, 7, 4,
                10, 9, 8,
                11, 10, 8,
                14, 13, 12,
                15, 14, 12,
                18, 17, 16,
                19, 18, 16,
                22, 21, 20,
                23, 22, 20 };

    NewUnit->CreateBuffer();
    NewUnit->GetMeshData().TextureName = _CubeMapTextureName;

    RenderUnits.push_back(NewUnit);
}

void Renderer::SetModelToSphere(int _XSlice, int _YSlice)
{
    std::shared_ptr<RenderBase> NewRenderUnit = std::make_shared<RenderBase>();

    const float Theta = -DirectX::XM_2PI / float(_XSlice);
    const float Phi = -DirectX::XM_PI / float(_YSlice);

    std::vector<EVertex>& Vertice = NewRenderUnit->GetMeshData().Vertices;

    for (int j = 0; j <= _YSlice; j++) 
    {
        DirectX::SimpleMath::Vector3 YStartPoint = DirectX::SimpleMath::Vector3::Transform(
            DirectX::SimpleMath::Vector3(0.0f, -1.0f, 0.0f), DirectX::SimpleMath::Matrix::CreateRotationZ(Phi * j));

        for (int i = 0; i <= _XSlice; i++) 
        {
            EVertex NewVertex;

            NewVertex.Position = DirectX::SimpleMath::Vector3::Transform(
                YStartPoint, DirectX::SimpleMath::Matrix::CreateRotationY(Theta * float(i)));

            NewVertex.Normal = NewVertex.Position; // ¿øÁ¡ÀÌ ±¸ÀÇ Áß½É
            NewVertex.Normal.Normalize();
            NewVertex.TexCoord =
                DirectX::SimpleMath::Vector2(float(i) / _XSlice, 1.0f - float(j) / _YSlice);

            Vertice.push_back(NewVertex);
        }
    }

    std::vector<uint32_t>& Indices = NewRenderUnit->GetMeshData().Indices;

    for (int j = 0; j < _YSlice; j++) {

        const int Offset = (_XSlice + 1) * j;

        for (int i = 0; i < _XSlice; i++) {

            Indices.push_back(Offset + i);
            Indices.push_back(Offset + i + _XSlice + 1);
            Indices.push_back(Offset + i + 1 + _XSlice + 1);

            Indices.push_back(Offset + i);
            Indices.push_back(Offset + i + 1 + _XSlice + 1);
            Indices.push_back(Offset + i + 1);
        }
    }

    NewRenderUnit->CreateBuffer();
    RenderUnits.push_back(NewRenderUnit);
}
