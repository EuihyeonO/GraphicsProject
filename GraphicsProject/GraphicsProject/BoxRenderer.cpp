#include "BoxRenderer.h"

BoxRenderer::BoxRenderer()
{
    Name = "BOX";
}

BoxRenderer::~BoxRenderer()
{
}

void BoxRenderer::Init()
{
    CreateVertexAndIndex();
    
    RenderBase::CreateVertexBuffer();
    RenderBase::CreateIndexBuffer();
    RenderBase::CreateConstantBuffer<Transform>(TransFormData);
}

void BoxRenderer::CreateVertexAndIndex()
{
    std::vector<DirectX::SimpleMath::Vector3> Positions;
    Positions.reserve(24);

    std::vector<DirectX::SimpleMath::Vector3> Colors;
    Colors.reserve(24);

    std::vector<DirectX::SimpleMath::Vector3> Normals;
    Normals.reserve(24);

    //À­¸é
    Positions.push_back({ -1.0f, 1.0f, -1.0f });
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    
    Colors.push_back({1.0f, 0.0f, 0.0f});
    Colors.push_back({1.0f, 0.0f, 0.0f});
    Colors.push_back({1.0f, 0.0f, 0.0f});
    Colors.push_back({1.0f, 0.0f, 0.0f});
   
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({0.0f, 1.0f, 0.0f});
    Normals.push_back({ 0.0f, 1.0f, 0.0f});

    //¾Æ·§¸é
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });
    Normals.push_back({ 0.0f, -1.0f, 0.0f });

    //¿ÞÂÊ
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ -1.0f, 1.0f, -1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });
    Normals.push_back({ -1.0f, 0.0f, 0.0f });

    //¿À¸¥ÂÊ
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });
    Normals.push_back({ 1.0f, 0.0f, 0.0f });

    //¾ÕÂÊ
    Positions.push_back({ -1.0f, 1.0f, 1.0f });
    Positions.push_back({ -1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, -1.0f, 1.0f });
    Positions.push_back({ 1.0f, 1.0f, 1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });
    Normals.push_back({ 0.0f, 0.0f, 1.0f });

    //µÞÂÊ
    Positions.push_back({ -1.0f, -1.0f, -1.0f });
    Positions.push_back({ -1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, 1.0f, -1.0f });
    Positions.push_back({ 1.0f, -1.0f, -1.0f });

    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });
    Colors.push_back({ 1.0f, 0.0f, 0.0f });

    Normals.push_back({ 0.0f, 0.0f, -1.0f });
    Normals.push_back({ 0.0f, 0.0f, -1.0f });
    Normals.push_back({ 0.0f, 0.0f, -1.0f });
    Normals.push_back({ 0.0f, 0.0f, -1.0f });


    for (size_t i = 0; i < Positions.size(); i++) 
    {
        Vertex NewVertex;
        NewVertex.Position = Positions[i];
        NewVertex.Color = Colors[i];
        NewVertex.Normal = Normals[i];

        Vertices.push_back(NewVertex);
    }

    Indices = { 0, 1, 2,
                0, 2, 3,
                4, 6, 5,
                4, 7, 6,
                8, 9, 10,
                8, 10, 11,
                12, 13, 14,
                12, 14, 15,
                16, 17, 18,
                16, 18, 19,
                20, 21, 22,
                20, 22, 23 };
}
