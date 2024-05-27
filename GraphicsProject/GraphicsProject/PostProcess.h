#pragma once
#include "BaseHeader.h"

class PostProcess
{

public:

	PostProcess();
	~PostProcess();

	PostProcess(const PostProcess& _Other) = delete;
	PostProcess(PostProcess&& _Other) noexcept = delete;
	PostProcess& operator=(const PostProcess& _Other) = delete;
	PostProcess& operator=(PostProcess&& _Other) noexcept = delete;

	void SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _SRV, int _Index = 0);
	void SetDepthStencil(Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _DSV)
	{
		DepthStencilView = _DSV;
	}

	void SetDoubleBuffer(RenderTarget _DoubleBuffer)
	{
		DoubleBufferRTV = _DoubleBuffer.RTV;
		DoubleBufferSRV = _DoubleBuffer.SRV;
	}

	virtual void Init();
	virtual void Render(float _DeltaTime);

protected:
	std::shared_ptr<class ScreenRenderer> PostProcessRenderer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> DoubleBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DoubleBufferSRV;
private:

};

