#pragma once
#include "PostProcess.h"

struct EBlurData
{
	int Width = 1600 * 0.25f;
	int Height = 900 * 0.25f;
	int Padding1 = 0;
	int Padding2 = 0;
};

class BloomPostProcess : public PostProcess
{

public:

	BloomPostProcess();
	~BloomPostProcess();

	BloomPostProcess(const BloomPostProcess& _Other) = delete;
	BloomPostProcess(BloomPostProcess&& _Other) noexcept = delete;
	BloomPostProcess& operator=(const BloomPostProcess& _Other) = delete;
	BloomPostProcess& operator=(BloomPostProcess&& _Other) noexcept = delete;

	virtual void Init() override;
	virtual void Render(float _Deltatime) override;

protected:
	
private:
	void CreateBlendState();

	Microsoft::WRL::ComPtr<ID3D11BlendState> BlendState;

	EBlurData BlurData;

	RenderTarget BlurTarget;
	RenderTarget DetectTarget;

	D3D11_VIEWPORT DownViewPort;
	D3D11_VIEWPORT UpViewPort;
};

