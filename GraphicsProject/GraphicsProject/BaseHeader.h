#pragma once

#include "Windows.h"

#include <iostream>
#include <string>
#include <SimpleMath.h>
#include <memory>

#include <vector>
#include <list>
#include <unordered_map>

#include <d3dcompiler.h>
#include <d3d11.h>
#include <wrl.h>

struct Vertex
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Color;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct Transform
{
	DirectX::SimpleMath::Matrix WorldMatrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix ViewMAtrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix ProjMatrix = DirectX::SimpleMath::Matrix();
};
