#include "ResourceManager.h"
#include "Importer.hpp"
#include "scene.h"
#include "EngineBase.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/postprocess.h>
#include <string_view>
#include <filesystem>

#pragma comment (lib, "assimp-vc143-mtd.lib")

std::string ResourceManager::ModelPath = "../3DModel/";
std::string ResourceManager::TexturePath = "../Texture/";

std::unordered_map<std::string, std::list<EMeshData>> ResourceManager::LoadedMeshes;
std::unordered_map<std::string, TextureData> ResourceManager::LoadedTextures;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

void ResourceManager::LoadTexture(const std::string& _TextureName)
{
    std::string Path = "../Texture/";
    Path += _TextureName;

    int Width = 0;
    int Height = 0;
    int Channels = 0;

    unsigned char* LoadedImage = stbi_load(Path.c_str(), &Width, &Height, &Channels, 0);
    if (LoadedImage == nullptr)
    {
        std::cout << "Image Load Failed" << std::endl;
		return;
    }

    std::vector<uint8_t> Image;

    Image.resize(Width * Height * Channels);
    memcpy(Image.data(), LoadedImage, Image.size() * sizeof(uint8_t));

    Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;

    D3D11_TEXTURE2D_DESC TexDesc = {};
    TexDesc.Width = Width;
    TexDesc.Height = Height;
    TexDesc.MipLevels = TexDesc.ArraySize = 1;
    TexDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    TexDesc.SampleDesc.Count = 1;
    TexDesc.Usage = D3D11_USAGE_IMMUTABLE;
    TexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = Image.data();
    InitData.SysMemPitch = TexDesc.Width * sizeof(uint8_t) * Channels;

    HRESULT Result = EngineBase::GetInstance().GetDevice()->CreateTexture2D(&TexDesc, &InitData, Texture.GetAddressOf());
    if (Result != S_OK)
    {
        std::cout << "CreateTexture2D failed " << std::endl;
        return;
    }

    Result = EngineBase::GetInstance().GetDevice()->CreateShaderResourceView(Texture.Get(), nullptr, SRV.GetAddressOf());
    if (Result != S_OK)
    {
        std::cout << "CreateTexture2D failed " << std::endl;
        return;
    }

    TextureData NewTextureData;
    NewTextureData.Texture = Texture;
    NewTextureData.ShaderResourceView = SRV;
    
    stbi_image_free(LoadedImage);

	LoadedTextures.insert({ _TextureName, NewTextureData });

    return;
}

void ResourceManager::Load(const std::string& _FileName, const std::string& _Path)
{
	if (LoadedMeshes.find(_FileName) != LoadedMeshes.end())
	{
		std::cout << _FileName << " is Already Loaded!" << std::endl;
		return;
	}

	Assimp::Importer Importer;

	std::string FullName = _Path + _FileName;
	const aiScene* Scene = Importer.ReadFile(FullName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	DirectX::SimpleMath::Matrix Transform;

	ProcessNode(Scene->mRootNode, Scene, LoadedMeshes[_FileName], Transform);
}

void ResourceManager::ProcessNode(aiNode* _Node, const aiScene* _Scene, std::list<EMeshData>& _MeshList, DirectX::SimpleMath::Matrix _Transform)
{
	DirectX::SimpleMath::Matrix Mat;
	ai_real* Temp = &_Node->mTransformation.a1;
	
	float* MatTemp = &Mat._11;
	for (int i = 0; i < 16; i++)
	{
		MatTemp[i] = Temp[i];
	}

	Mat = Mat.Transpose() * _Transform;

	for (UINT i = 0; i < _Node->mNumMeshes; i++)
	{
		aiMesh* Mesh = _Scene->mMeshes[_Node->mMeshes[i]];
		ProcessMesh(Mesh, _Scene, _MeshList);

		EMeshData& NewMesh = _MeshList.back();
		for (auto& Vertex : NewMesh.Vertices)
		{
			Vertex.Position = DirectX::SimpleMath::Vector3::Transform(Vertex.Position, Mat);
		}
	}

	for (UINT i = 0; i < _Node->mNumChildren; i++)
	{
		ProcessNode(_Node->mChildren[i], _Scene, _MeshList, Mat);
	}
}

void ResourceManager::ProcessMesh(aiMesh* _Mesh, const aiScene* _Scene, std::list<EMeshData>& _MeshList)
{
	EMeshData NewMesh;
	NewMesh.Indices;

	UINT IndicesCount = 0;
	for (UINT i = 0; i < _Mesh->mNumFaces; i++)
	{
		IndicesCount += _Mesh->mFaces[i].mNumIndices;
	}

	NewMesh.Vertices.reserve(_Mesh->mNumVertices);
	NewMesh.Indices.reserve(IndicesCount);

	for (UINT i = 0; i < _Mesh->mNumVertices; i++)
	{
		EVertex NewVertex;

		NewVertex.Position.x = _Mesh->mVertices[i].x;
		NewVertex.Position.y = _Mesh->mVertices[i].y;
		NewVertex.Position.z = _Mesh->mVertices[i].z;

		NewVertex.Normal.x = _Mesh->mNormals[i].x;
		NewVertex.Normal.y = _Mesh->mNormals[i].y;
		NewVertex.Normal.z = _Mesh->mNormals[i].z;

		NewVertex.Normal.Normalize();

		if (_Mesh->mTextureCoords[0])
		{
			NewVertex.TexCoord.x = (float)_Mesh->mTextureCoords[0][i].x;
			NewVertex.TexCoord.y = (float)_Mesh->mTextureCoords[0][i].y;
		}

		NewMesh.Vertices.push_back(NewVertex);
	}

	for (UINT i = 0; i < _Mesh->mNumFaces; i++) 
	{
		aiFace& Face = _Mesh->mFaces[i];
		
		for (UINT j = 0; j < Face.mNumIndices; j++)
		{
			NewMesh.Indices.push_back(Face.mIndices[j]);
		}
	}

	if (_Mesh->mMaterialIndex >= 0) 
	{
		aiMaterial* Material = _Scene->mMaterials[_Mesh->mMaterialIndex];

		if (Material->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
		{
			aiString TexturePath;
			Material->GetTexture(aiTextureType_DIFFUSE, 0, &TexturePath);
		
			std::string TextureName = std::string(std::filesystem::path(TexturePath.C_Str()).filename().string());

			NewMesh.TextureName = TextureName;

			if (LoadedTextures.find(TextureName) == LoadedTextures.end())
			{
				LoadTexture(TextureName);
			}
		}
	}

	_MeshList.push_back(NewMesh);
}
