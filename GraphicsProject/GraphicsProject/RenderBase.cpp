#include "RenderBase.h"

RenderBase::RenderBase()
{
}

RenderBase::~RenderBase()
{
}

void RenderBase::CreateVertexBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = { 0, };
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));

    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; 
    bufferDesc.ByteWidth = UINT(sizeof(Vertex) * Vertices.size());
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0; 
    bufferDesc.StructureByteStride = sizeof(Vertex);

    D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, }; 
    vertexBufferData.pSysMem = Vertices.data();
    vertexBufferData.SysMemPitch = 0;
    vertexBufferData.SysMemSlicePitch = 0;

    const HRESULT Result =
        EngineBase::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, &vertexBufferData, VertexBuffer.GetAddressOf());

    if (Result != S_OK) 
    {
        std::cout << Name << " :" << "CreateVertexBuffer() failed." << std::hex << "\nResult : " << Result  << std::endl;
    };
}

void RenderBase::CreateIndexBuffer()
{
    D3D11_BUFFER_DESC bufferDesc = {0, };
    bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
    bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * Indices.size());
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0; 
    bufferDesc.StructureByteStride = sizeof(uint16_t);

    D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
    indexBufferData.pSysMem = Indices.data();
    indexBufferData.SysMemPitch = 0;
    indexBufferData.SysMemSlicePitch = 0;

    HRESULT Result = 
        EngineBase::GetInstance()->GetDevice()->CreateBuffer(&bufferDesc, &indexBufferData, IndexBuffer.GetAddressOf());

    if (Result != S_OK)
    {
        std::cout << Name << " :" << "CreateIndexBuffer() failed." << std::hex << "\nResult : " << Result << std::endl;
    };
}
