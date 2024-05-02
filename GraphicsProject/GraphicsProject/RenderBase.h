#pragma once
#include "EngineBase.h"

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
	virtual void Update() = 0;
	
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	template <typename DataType>
	void CreateConstantBuffer(DataType& _Data)
	{
		D3D11_BUFFER_DESC CBDesc;
		CBDesc.ByteWidth = sizeof(DataType);
		CBDesc.Usage = D3D11_USAGE_DYNAMIC;
		CBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		CBDesc.MiscFlags = 0;
		CBDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = &_Data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		HRESULT Result =
			EngineBase::GetInstance().GetDevice()->CreateBuffer(&CBDesc, &InitData, ConstantBuffer.GetAddressOf());

		if (Result != S_OK)
		{
			std::cout << Name << " :" << "CreateConstantBuffer() failed." << std::hex << "\nResult : " << Result << std::endl;
		};
	}

	template <typename T>
	static std::shared_ptr<T> CreateRenderer()
	{
		std::shared_ptr<class RenderBase> NewRenderer = std::make_shared<T>();
		NewRenderer->Init();

		EngineBase::GetInstance().AddRenderer(NewRenderer);

		return std::dynamic_pointer_cast<T>(NewRenderer);
	}


	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBertexBuffer()
	{
		return VertexBuffer;
	}


	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}


	Microsoft::WRL::ComPtr<ID3D11Buffer> GetConstantBuffer()
	{
		return ConstantBuffer;
	}

protected:
	std::vector<struct Vertex> Vertices;
	std::vector<uint16_t> Indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;

	Transform TransFormData;

	std::string Name = "";
private:
};

