#version 410

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec3 lightIntensity;

out vec3 test;
out float alpha;
out vec2 TexCoord;

struct LightInfo
{
    vec4 pos;  //light pos
    vec3 La;   //ambient of light
    vec3 Ld;   //diffuse of light
    vec3 Ls;   //spec of light

};

uniform LightInfo Light;


struct MaterialInfo
{
    vec3 Ka;    //Ambient reflectivity
    vec3 Kd;    //diffuse reflectivity
    vec3 Ks;    //specular reflectivity
    float Shininess; //specular shininess factor
};

uniform MaterialInfo Material;

uniform float m_alpha; //light col

uniform mat4 MV;
uniform mat3 normalMatrix;
uniform mat4 MVP;

void getEyeSpace(out vec3 norm, out vec4 pos)
{
    norm = normalize( normalMatrix * inNormal);
    pos = MV * vec4(inVert,1.0);
}

vec3 phongModel(vec3 norm, vec4 pos)
{
    vec3 s = normalize(vec3(Light.pos - pos));
    //direction towards viewer, (negated because the viewer is at the origin)
    vec3 v = normalize(-pos.xyz); //swizzling
    //compute pure reflection
    vec3 r = reflect(-s, norm);
    //compute ambient
    vec3 ambient = Light.La * Material.Ka;

    //get perpendicular
    float sDotn = max(dot(s,norm),0.0);

    //compute diffuse
    vec3 diffuse = Light.Ld * Material.Kd * sDotn;

    //compute specular
    vec3 spec = vec3(0.0);

    if (sDotn > 0.0)
        spec = Light.Ls * Material.Ks * pow(max(dot(r,v),0.0),
                                            Material.Shininess);

    //Is = LsKs(r.v)^f

    return ambient + diffuse + spec;
}



void main()
{

    vec3 eyeNorm;
    vec4 eyePos;

    TexCoord = inUV;

    getEyeSpace( eyeNorm, eyePos);

    lightIntensity = phongModel( eyeNorm, eyePos);

    gl_Position = MVP*vec4(inVert,1.0);

    alpha = m_alpha;

}
