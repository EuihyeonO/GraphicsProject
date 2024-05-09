#include "RenderBase.h"

RenderBase::RenderBase()
{
}

RenderBase::~RenderBase()
{
}

void RenderBase::Init()
{
    isCallInit = true;

    CreateConstantBuffer(EngineBase::GetInstance().GetWorldLight());
}

void RenderBase::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC BufferDesc = { 0, };
    ZeroMemory(&BufferDesc, sizeof(BufferDesc));

    BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    BufferDesc.ByteWidth = UINT(sizeof(Vertex) * Vertices.size());
    BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    BufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA VertexBufferData = { 0, }; 
    VertexBufferData.pSysMem = Vertices.data();
    VertexBufferData.SysMemPitch = 0;
    VertexBufferData.SysMemSlicePitch = 0;

    const HRESULT Result =
        EngineBase::GetInstance().GetDevice()->CreateBuffer(&BufferDesc, &VertexBufferData, VertexBuffer.GetAddressOf());

    if (Result != S_OK) 
    {
        std::cout << Name << " :" << "CreateVertexBuffer() failed." << std::hex << "\nResult : " << Result  << std::endl;
    };
}

void RenderBase::CreateIndexBuffer()
{
    D3D11_BUFFER_DESC BufferDesc = {0, };
    BufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    BufferDesc.ByteWidth = UINT(sizeof(uint16_t) * Indices.size());
    BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    BufferDesc.CPUAccessFlags = 0;
    BufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA IndexBufferData = { 0 };
    IndexBufferData.pSysMem = Indices.data();
    IndexBufferData.SysMemPitch = 0;
    IndexBufferData.SysMemSlicePitch = 0;

    HRESULT Result = 
        EngineBase::GetInstance().GetDevice()->CreateBuffer(&BufferDesc, &IndexBufferData, IndexBuffer.GetAddressOf());

    if (Result != S_OK)
    {
        std::cout << Name << " :" << "CreateIndexBuffer() failed." << std::hex << "\nResult : " << Result << std::endl;
    };
}


