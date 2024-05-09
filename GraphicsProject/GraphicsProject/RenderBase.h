#pragma once
#include "EngineBase.h"

struct ConstantBufferData
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	
	void* Data = nullptr;
	UINT DataSize = 0;
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
	virtual void Init();
	virtual void Render(float _DeltaTime) = 0;
	virtual void Update(float _DeltaTime) = 0;
	
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

		Microsoft::WRL::ComPtr<ID3D11Buffer> NewBuffer;

		HRESULT Result =
			EngineBase::GetInstance().GetDevice()->CreateBuffer(&CBDesc, &InitData, NewBuffer.GetAddressOf());

		if (Result != S_OK)
		{
			std::cout << Name << " :" << "CreateConstantBuffer() failed." << std::hex << "\nResult : " << Result << std::endl;
		};

		ConstantBufferData NewBufferData;
		NewBufferData.ConstantBuffer = NewBuffer;
		NewBufferData.Data = reinterpret_cast<void*>(&_Data);
		NewBufferData.DataSize = sizeof(_Data);

		ConstantBuffers.push_back(NewBufferData);
	}

	template <typename T>
	static std::shared_ptr<T> CreateRenderer()
	{
		std::shared_ptr<class RenderBase> NewRenderer = std::make_shared<T>();
		NewRenderer->Init();

		if (NewRenderer->isCallInitFunction() == false)
		{
			std::cout << NewRenderer->Name << " : RenderBase::Init() is not called. " << std::endl;
		}

		EngineBase::GetInstance().AddRenderer(NewRenderer);

		return std::dynamic_pointer_cast<T>(NewRenderer);
	}

	void SetTexture(const std::string& _TextureName)
	{
		TextureName = _TextureName;
	}

	void SetSampler(const std::string& _SamplerName)
	{
		SamplerName = _SamplerName;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBertexBuffer()
	{
		return VertexBuffer;
	}


	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}


	const std::list<ConstantBufferData>& GetConstantBuffer()
	{
		return ConstantBuffers;
	}

	bool isCallInitFunction()
	{
		return isCallInit;
	}

protected:
	std::vector<struct Vertex> Vertices;
	std::vector<uint16_t> Indices;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    std::list<ConstantBufferData> ConstantBuffers;

	Transform TransFormData;

	std::string Name = "";

	std::string TextureName = "";
	std::string SamplerName = "";

private:
	bool isCallInit = false;
};

