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
	void Render(float _DeltaTime);
	
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	template <typename T>
	static std::shared_ptr<T> CreateRenderer()
	{
		std::shared_ptr<class Renderer> NewRenderer = std::make_shared<T>();
		NewRenderer->Init();

		if (NewRenderer->isCallInitFunction() == false)
		{
			std::cout << NewRenderer->Name << " : RenderBase::Init() is not called. " << std::endl;
		}

		EngineBase::GetInstance().AddRenderer(NewRenderer);

		return std::dynamic_pointer_cast<T>(NewRenderer);
	}

	void RenderSetting();

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



	const std::wstring& GetVSShaderName()
	{
		return VSShader;
	}

	const std::wstring& GetPSShaderName()
	{
		return PSShader;
	}
	
	EMeshData& GetMeshData()
	{
		return MeshData;
	}

protected:
	EMeshData MeshData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;


	std::wstring VSShader = L"";
	std::wstring PSShader = L"";

	std::string SamplerName = "";

private:

	std::shared_ptr<class Renderer> Owner = nullptr;
};

