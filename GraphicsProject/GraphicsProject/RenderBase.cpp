#include "RenderBase.h"
#include "Renderer.h"
#include "ResourceManager.h"

RenderBase::RenderBase()
{
    SRVs.push_back(MySRV);
}

RenderBase::~RenderBase()
{
}

void RenderBase::CreateBuffer()
{
    CreateVertexBuffer();
    CreateIndexBuffer();
}

void RenderBase::Render(float _DeltaTime)
{
    UINT IndexCount = (UINT)MeshData.Indices.size();
    EngineBase::GetInstance().GetContext()->DrawIndexed(IndexCount, 0, 0);
    
    ID3D11ShaderResourceView* SRV[2] = {NULL, };
    EngineBase::GetInstance().GetContext()->PSSetShaderResources(0, 2, SRV);
}

void RenderBase::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC BufferDesc = { 0, };
    ZeroMemory(&BufferDesc, sizeof(BufferDesc));

    BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    BufferDesc.ByteWidth = UINT(sizeof(EVertex) * MeshData.Vertices.size());
    BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    BufferDesc.StructureByteStride = sizeof(EVertex);

    D3D11_SUBRESOURCE_DATA VertexBufferData = { 0, }; 
    VertexBufferData.pSysMem = MeshData.Vertices.data();
    VertexBufferData.SysMemPitch = 0;
    VertexBufferData.SysMemSlicePitch = 0;

    const HRESULT Result =
        EngineBase::GetInstance().GetDevice()->CreateBuffer(&BufferDesc, &VertexBufferData, VertexBuffer.GetAddressOf());

    if (Result != S_OK) 
    {
        std::string Name = (Owner == nullptr) ? "None" : Owner->Name;
        std::cout << Name << " :" << "CreateVertexBuffer() failed." << std::hex << "\nResult : " << Result << std::endl;
    };
}

void RenderBase::CreateIndexBuffer()
{
    D3D11_BUFFER_DESC BufferDesc = {0, };
    BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    BufferDesc.ByteWidth = UINT(sizeof(uint32_t) * MeshData.Indices.size());
    BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    BufferDesc.StructureByteStride = sizeof(uint32_t);

    D3D11_SUBRESOURCE_DATA IndexBufferData = { 0 };
    IndexBufferData.pSysMem = MeshData.Indices.data();
    IndexBufferData.SysMemPitch = 0;
    IndexBufferData.SysMemSlicePitch = 0;

    HRESULT Result = 
        EngineBase::GetInstance().GetDevice()->CreateBuffer(&BufferDesc, &IndexBufferData, IndexBuffer.GetAddressOf());

    if (Result != S_OK)
    {
        std::string Name = (Owner == nullptr) ? "None" : Owner->Name;
        std::cout << Name << " :" << "CreateIndexBuffer() failed." << std::hex << "\nResult : " << Result << std::endl;
    };
}

void RenderBase::RenderSetting()
{
    UINT Stride = sizeof(EVertex);
    UINT Offset = 0;

    VertexShaderData VSData = EngineBase::GetInstance().GetVertexShaderData(VSShader);
    Microsoft::WRL::ComPtr<ID3D11PixelShader> PS = EngineBase::GetInstance().GetPixelShaderData(PSShader);

    EngineBase::GetInstance().GetContext()->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &Stride, &Offset);
    EngineBase::GetInstance().GetContext()->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    EngineBase::GetInstance().GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    EngineBase::GetInstance().GetContext()->VSSetShader(VSData.VertexShader.Get(), 0, 0);
    EngineBase::GetInstance().GetContext()->IASetInputLayout(VSData.InputLayout.Get());
    EngineBase::GetInstance().GetContext()->PSSetShader(PS.Get(), 0, 0);

    //추후 텍스쳐 여러개 세팅할 수도 있다.
    Microsoft::WRL::ComPtr<ID3D11SamplerState> Sampler = EngineBase::GetInstance().GetSampler(SamplerName);

    EngineBase::GetInstance().GetContext()->PSSetShaderResources(0, SRVs.size(), SRVs.data()->GetAddressOf());
    EngineBase::GetInstance().GetContext()->PSSetSamplers(0, 1, Sampler.GetAddressOf());
}

void RenderBase::SetTexture(const std::string& _TextureName)
{
    MeshData.TextureName = _TextureName;

    const TextureData& _TextureData = ResourceManager::GetTexture(_TextureName);
    SRVs[0] = _TextureData.ShaderResourceView;
}


