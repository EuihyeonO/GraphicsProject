#pragma once

#include "Windows.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <functional>
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
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct Transform
{
	DirectX::SimpleMath::Matrix WorldMatrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix ViewMAtrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix ProjMatrix = DirectX::SimpleMath::Matrix();
};
