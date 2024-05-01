#pragma once
#include "EngineBase.h"

struct Vertex
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Color;
	DirectX::SimpleMath::Vector3 Normal;
};

class RenderBase
{

public:
	RenderBase();
	~RenderBase();

	RenderBase(const RenderBase& _Other) = delete;
	RenderBase(RenderBase&& _Other) noexcept = delete;
	RenderBase& operator=(const RenderBase& _Other) = delete;
	RenderBase& operator=(RenderBase&& _Other) noexcept = delete;

public:
	virtual void Init() = 0;
	virtual void Render() = 0;
	
	void CreateVertexBuffer();
	void CreateIndexBuffer();

protected:
	std::vector<Vertex> Vertices;
	std::vector<uint16_t> Indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;

	std::string Name = "";
private:
};

