#pragma once
#include "PostProcess.h"

struct EBloomData
{
	int Width = 1600;
	int Height = 900;
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
	EBloomData BloomData;
};

