#define LIGHT_NUM 3

struct Light
{
    float3 Strength;
    float FallOffStart;

    float3 Direction;
    float FallOffEnd;

    float3 Position;
    float SpotPower;
};

float CalDiffuseLight(Light _Light, float3 _Normal)
{
    float3 LightDir = -_Light.Direction;
    LightDir = normalize(LightDir);
    
    float3 Normal = normalize(_Normal);
    
    float CosTheta = dot(LightDir, Normal);
    
    return CosTheta;
}

float3 CalSpecular_BlinnPhong()
{
    return (float3)0.0f;
}

float CalSpecular_Phong(Light _Light, float3 _Normal, float3 _EyeDir)
{
    float3 LightDir = _Light.Direction;
    LightDir = normalize(LightDir);
    
    float3 Normal = _Normal;
    Normal = normalize(Normal);
    
    float3 ReflectedLightDir = reflect(LightDir, Normal);
    
    float3 EyeDir = _EyeDir;
    EyeDir = normalize(EyeDir);
    
    float Specular = max(0.0f, dot(ReflectedLightDir, EyeDir));
    Specular = pow(Specular, 32.0f);
    
    return max(Specular, 0.0f);
}

float3 BlinnPhong()
{
    //float3 Light = CalDiffuseLight() + CalSpecular_BlinnPhong();
    //return Light;
    return (float3) 0.0f;
    
}

float3 Phong()
{
    //float3 Light = CalDiffuseLight() + CalSpecular_Phong();
    //return Light;
    return (float3) 0.0f;
}