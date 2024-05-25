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
	void CreateBuffer();

	void Render(float _DeltaTime);
	
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	void RenderSetting();

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

	void SetMesh(const EMeshData& _Mesh)
	{
		MeshData = _Mesh;
	}

	void SetTexture(const std::string& _TextureName);

	void SetTextureToSRV(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _SRV)
	{
		MySRV = _SRV;
	}

protected:
	EMeshData MeshData;

	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> MySRV;

	std::wstring VSShader = L"";
	std::wstring PSShader = L"";

	std::string SamplerName = "";

private:
	std::shared_ptr<class Renderer> Owner = nullptr;
};

