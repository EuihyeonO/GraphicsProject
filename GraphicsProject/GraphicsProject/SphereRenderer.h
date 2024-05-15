#pragma once
#include "Renderer.h"

class SphereRenderer : public Renderer
{

public:

	SphereRenderer();
	~SphereRenderer();

	SphereRenderer(const SphereRenderer& _Other) = delete;
	SphereRenderer(SphereRenderer&& _Other) noexcept = delete;
	SphereRenderer& operator=(const SphereRenderer& _Other) = delete;
	SphereRenderer& operator=(SphereRenderer&& _Other) noexcept = delete;

protected:
	virtual void Init() override;
	void Update(float _DeltaTime);
private:

};

