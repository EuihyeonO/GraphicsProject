#pragma once
#include "EngineBase.h"

enum class EShaderType
{
	VSShader,
	PSShader,
};

struct EConstantBufferData
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	
	void* Data = nullptr;
	UINT DataSize = 0;
};

struct EMeshData
{
	std::vector<struct EVertex> Vertices;
	std::vector<uint16_t> Indices;
	std::string TextureName = "";
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
	void CreateConstantBuffer(EShaderType _ShaderType, const std::wstring& _ShaderName, DataType& _Data)
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

		EConstantBufferData NewBufferData;
		NewBufferData.ConstantBuffer = NewBuffer;
		NewBufferData.Data = reinterpret_cast<void*>(&_Data);
		NewBufferData.DataSize = sizeof(_Data);

		if (_ShaderType == EShaderType::VSShader)
		{
			VSConstantBuffers[_ShaderName].push_back(NewBufferData);
		}
		else if (_ShaderType == EShaderType::PSShader)
		{
			PSConstantBuffers[_ShaderName].push_back(NewBufferData);
		}
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
		MeshData.TextureName = _TextureName;
	}

	void SetSampler(const std::string& _SamplerName)
	{
		SamplerName = _SamplerName;
	}


	void SetVSShader(const std::wstring& _VSShaderName)
	{
		VSShader = _VSShaderName;
	}

	void SetPSShader(const std::wstring& _PSShaderName)
	{
		PSShader = _PSShaderName;
	}

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBertexBuffer()
	{
		return VertexBuffer;
	}


	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}


	const std::unordered_map<std::wstring, std::list<EConstantBufferData>>& GetVSConstantBuffer()
	{
		return VSConstantBuffers;
	}

	const std::unordered_map<std::wstring, std::list<EConstantBufferData>>& GetPSConstantBuffer()
	{
		return PSConstantBuffers;
	}

	bool isCallInitFunction()
	{
		return isCallInit;
	}

protected:

	EMeshData MeshData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

	std::unordered_map<std::wstring, std::list<EConstantBufferData>> VSConstantBuffers;
	std::unordered_map<std::wstring, std::list<EConstantBufferData>> PSConstantBuffers;

	EMaterial MaterialData;
	ETransform TransFormData;

	std::string Name = "";

	std::wstring VSShader = L"";
	std::wstring PSShader = L"";

	std::string SamplerName = "";

private:
	bool isCallInit = false;
};

