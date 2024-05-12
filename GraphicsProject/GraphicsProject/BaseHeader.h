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

struct EVertex
{
	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Vector3 Normal;
	DirectX::SimpleMath::Vector2 TexCoord;
};

struct ETransform
{
	DirectX::SimpleMath::Matrix WorldMatrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix ViewMAtrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix ProjMatrix = DirectX::SimpleMath::Matrix();
	DirectX::SimpleMath::Matrix InvTranspose = DirectX::SimpleMath::Matrix();
};

struct ELight
{
	DirectX::SimpleMath::Vector3 Strength = { 1.0f, 1.0f, 1.0f };
	float FallOffStart = 1.0f;

	DirectX::SimpleMath::Vector3 Direction = { 0.0f, 0.0f, 1.0f };
	float FallOffEnd = 10.0f;

	DirectX::SimpleMath::Vector3 Position = { 0.0f, 0.0f, -2.0f };
	float SpotPower = 1.0f;
};

struct EMaterial
{
	DirectX::SimpleMath::Vector3 Ambient = { 0.1f, 0.1f, 0.1f };
	float Shininess = 50.0f;

	DirectX::SimpleMath::Vector3 Diffuse = { 0.5f, 0.5f, 0.5f };
	float Padding1;

	DirectX::SimpleMath::Vector3 Specular = { 1.0f, 1.0f, 1.0f };
	float Padding2;
};

#define LIGHT_NUM 3

struct EWorldLight
{
	DirectX::SimpleMath::Vector3 EyeWorld;
	float Padding = 0.0f;

	ELight Lights[LIGHT_NUM];
};