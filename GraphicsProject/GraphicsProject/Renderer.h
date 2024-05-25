#pragma once

#include "BaseHeader.h"
#include "RenderBase.h"

struct ERimLight
{
	DirectX::SimpleMath::Vector3 RimColor = { 1.0f, 1.0f, 1.0f };
	float RimPower = 3.0f;
	
	float RimStrength = 3.0f;
	float Padding[3];
};

class Renderer
{

public:

	Renderer();
	~Renderer();

	Renderer(const Renderer& _Other) = delete;
	Renderer(Renderer&& _Other) noexcept = delete;
	Renderer& operator=(const Renderer& _Other) = delete;
	Renderer& operator=(Renderer&& _Other) noexcept = delete;

	virtual void Init();

	virtual void Update(float _DeltaTime);
	void ConstantBufferUpdate();
	void Render(float _DeltaTime);
	void SetConstantBuffer(const std::wstring& _VSShaderName, const std::wstring& _PSShaderName);

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

	bool isCallInitFunction()
	{
		return isCallInit;
	}

	void AddRenderUnit(std::shared_ptr<class RenderBase> _Unit)
	{
		RenderUnits.push_back(_Unit);
	}

	void SetVSShader(const std::wstring& _Shadername);
	void SetPSShader(const std::wstring& _Shadername);
	void SetSampler(const std::string& _Sampler);

	void SetModel(const std::string& _Name);
	void SetModelToCube(const std::string& _CubeMapTextureName);
	void SetModelToSphere(int _XSlice, int _YSlice);
	void SetModelToSquare(float _Width);

	std::list<std::shared_ptr<class RenderBase>>& GetUnits()
	{
		return RenderUnits;
	}

	template <typename T>
	static std::shared_ptr<T> CreateRenderer()
	{
		std::shared_ptr<class Renderer> NewRenderer = std::make_shared<T>();
		NewRenderer->Init();

		if (NewRenderer->isCallInitFunction() == false)
		{
			std::cout << NewRenderer->Name << " : NewRenderer::Init() is not called. " << std::endl;
		}

		EngineBase::GetInstance().AddRenderer(NewRenderer);

		return std::dynamic_pointer_cast<T>(NewRenderer);
	}

	std::string Name = "";

protected:
	ERimLight RimLightData;
	EMaterial MaterialData;
	ETransform TransFormData;

private:
	std::list<std::shared_ptr<class RenderBase>> RenderUnits;

	std::unordered_map<std::wstring, std::list<EConstantBufferData>> VSConstantBuffers;
	std::unordered_map<std::wstring, std::list<EConstantBufferData>> PSConstantBuffers;

	bool isCallInit = false;
};

