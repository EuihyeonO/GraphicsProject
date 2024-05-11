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

struct Material
{
    float3 Ambient;
    float Shininess;

    float3 Diffuse;
    float Padding1;

    float3 Specular;
    float Padding2;
};

float Attenuation(float _Dist, float _FallOffStart, float _FallOffEnd)
{
    return saturate((_FallOffEnd - _Dist) / (_FallOffEnd - _FallOffStart));
}

float3 CalDirectionalLight(Light _Light, float3 _Normal)
{
    float3 LightDir = -_Light.Direction;
    LightDir = normalize(LightDir);
    
    float3 Normal = normalize(_Normal);
    float CosTheta = dot(LightDir, Normal);
    
    return (float3)max(CosTheta, 0.0f);
}

float3 CalPointLight(Light _Light, float3 _WorldPos, float3 _Normal)
{
    float3 LightVec = _Light.Position - _WorldPos;
    float Dist = length(LightVec);
    
    if(Dist > _Light.FallOffEnd)
    {
        return (float3) 0.0f;
    }
    else
    {
        LightVec /= Dist; //Normalize;
        
        float3 Normal = normalize(_Normal);
        
        float CosTheta = dot(Normal, LightVec);
        CosTheta = max(CosTheta, 0.0f);
        
        float Att = Attenuation(Dist, _Light.FallOffStart, _Light.FallOffEnd);
        float3 ResultLight = CosTheta * _Light.Strength * Att;
        
        return ResultLight;
    }
}

float3 CalSpotLight(Light _Light, float3 _WorldPos, float3 _Normal)
{
    float3 LightVec = _Light.Position - _WorldPos;
    float Dist = length(LightVec);
    
    if (Dist > _Light.FallOffEnd)
    {
        return (float3) 0.0f;
    }
    else
    {
        LightVec /= Dist; //Normalize;
        
        float3 Normal = normalize(_Normal);
        
        float CosTheta = dot(Normal, LightVec);
        CosTheta = max(CosTheta, 0.0f);
        
        float Att = Attenuation(Dist, _Light.FallOffStart, _Light.FallOffEnd);
        float3 LightStrength = CosTheta * _Light.Strength * Att;
        
        float SpotFactor = dot(LightVec, -_Light.Direction);
        SpotFactor = max(SpotFactor, 0.0f);
        SpotFactor = pow(SpotFactor, _Light.SpotPower);
        
        float3 ResultLight = LightStrength * SpotFactor;
        
        return ResultLight;
    }
}

float3 CalSpecular_BlinnPhong(Light _Light, Material _Mat, float3 _EyeDir, float3 _Normal)
{
    float3 LightVec = normalize(-_Light.Direction);
    float3 EyeDir = normalize(EyeDir);
    
    float3 HalfWay = normalize(_EyeDir + LightVec);
    float3 Normal = normalize(_Normal);
    
    float HalfDotN = dot(HalfWay, Normal);
    
    float3 Specular = max(HalfDotN, 0.0f);
    
    Specular = pow(Specular, _Mat.Shininess);
    Specular *= _Mat.Specular;
    
    return Specular;
}

float CalSpecular_Phong(Light _Light, Material _Mat, float3 _Normal, float3 _EyeDir)
{
    float3 LightDir = _Light.Direction;
    LightDir = normalize(LightDir);
    
    float3 Normal = _Normal;
    Normal = normalize(Normal);
    
    float3 ReflectedLightDir = reflect(LightDir, Normal);
    
    float3 EyeDir = normalize(_EyeDir);
    
    float Specular = max(0.0f, dot(ReflectedLightDir, EyeDir));
    Specular = pow(Specular, _Mat.Shininess);
    
    return max(Specular, 0.0f);
}

float3 Phong(Light _Light, Material _Mat, float3 _LStrength, float3 _Normal, float3 _EyeDir)
{
    float3 LightStrength = _LStrength;
    float3 Specular = _Mat.Specular * CalSpecular_Phong(_Light, _Mat, _Normal, _EyeDir);
    
    float3 ReturnValue = _Mat.Ambient + (_Mat.Diffuse + Specular) * LightStrength;
    
    return ReturnValue;
}

float3 BlinnPhong(Light _Light, Material _Mat, float3 _LStrength, float3 _Normal, float3 _EyeDir)
{
    float3 LightStrength = _LStrength;
    float3 Specular = _Mat.Specular * CalSpecular_BlinnPhong(_Light, _Mat, _Normal, _EyeDir);
    
    float3 ReturnValue = _Mat.Ambient + (_Mat.Diffuse + Specular) * LightStrength;
    
    return ReturnValue;
}
