#pragma once
#include "Renderer.h"

class BoxRenderer : public Renderer
{

public:
	
	BoxRenderer();
	~BoxRenderer();

	BoxRenderer(const BoxRenderer& _Other) = delete;
	BoxRenderer(BoxRenderer&& _Other) noexcept = delete;
	BoxRenderer& operator=(const BoxRenderer& _Other) = delete;
	BoxRenderer& operator=(BoxRenderer&& _Other) noexcept = delete;

public:
	virtual void Init() override;
	void Update(float _DeltaTime);
protected:

private:
	void CreateVertexAndIndex();
};

