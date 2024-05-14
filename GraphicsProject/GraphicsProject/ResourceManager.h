#pragma once
#include "BaseHeader.h"

enum ETextureType
{
	Diffuse,
	CubeMap,
};

class ResourceManager
{

public:

	ResourceManager();
	~ResourceManager();

	ResourceManager(const ResourceManager& _Other) = delete;
	ResourceManager(ResourceManager&& _Other) noexcept = delete;
	ResourceManager& operator=(const ResourceManager& _Other) = delete;
	ResourceManager& operator=(ResourceManager&& _Other) noexcept = delete;

	static bool isLoadedModel(const std::string& _ModelName)
	{
		return LoadedMeshes.find(_ModelName) != LoadedMeshes.end();
	}

	static void LoadTexture(const std::string& _TextureName, ETextureType _Type = ETextureType::Diffuse);

	static const std::list<EMeshData>& GetLoadedMeshList(const std::string& _ModelName)
	{
		if (isLoadedModel(_ModelName) == false)
		{
			std::cout << _ModelName << " is Not Loaded!" << std::endl;
			return std::list<EMeshData>();
		}

		return LoadedMeshes.find(_ModelName)->second;
	}

	static const TextureData& GetTexture(const std::string& _TextureName)
	{
		if (LoadedTextures.find(_TextureName) == LoadedTextures.end())
		{
			std::cout << "No Exist TextureData" << std::endl;
		}

		return LoadedTextures[_TextureName];
	}

	static void Load(const std::string& _FileName, const std::string& _Path = ModelPath);

	static std::string ModelPath;
	static std::string TexturePath;

protected:

private:
	static void ProcessNode(struct aiNode* _Node, const struct aiScene* _Scene, std::list<EMeshData>& _MeshList, DirectX::SimpleMath::Matrix _Transform);
	static void ProcessMesh(struct aiMesh* _Mesh, const struct aiScene* _Scene, std::list<EMeshData>& _MeshList);

	static void LoadDiffuseTexture(const std::string& _TextureName);
	static void LoadCubeMapTexture(const std::string& _TextureName);

	static std::unordered_map<std::string, TextureData> LoadedTextures;
	static std::unordered_map<std::string, std::list<EMeshData>> LoadedMeshes;
};

