#version 410

layout (location = 0) in vec3 inVert;
layout (location = 2) in vec3 inNormal;

out vec3 Colour;

out float alpha;

struct LightInfo
{
    vec4 pos;  //light pos
    vec3 intensity;   //intensity of light

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

vec3 phongModel(int lightIndex, vec3 norm, vec4 pos)
{
    vec3 s;

    //checking if light is directional or not

    if( Light.pos.w == 0)
        s = normalize(vec3(Light.pos));
        //directional light, used as direction towards light source
    else
        s = normalize(vec3(Light.pos-pos));
        //not directional light,light pos treated as location in eye coords
    //direction towards light source, (negated because the viewer is at the origin)



    vec3 v = normalize(-pos.xyz); //swizzling
    //compute pure reflection
    vec3 r = reflect(-s, norm);

    vec3 I = Light.intensity;
    //get perpendicular
    float sDotn = max(dot(s,norm),0.0);

    //compute diffuse
    vec3 diffuse =  Material.Kd * sDotn;

    //compute specular
    vec3 spec = vec3(0.0);

    if (sDotn > 0.0)
        spec =  Material.Ks * pow(max(dot(r,v),0.0),
                                            Material.Shininess);

    //Is = LsKs(r.v)^f


    return I * (Material.Ka + diffuse + spec);
}

void main()
{

    vec3 eyeNorm;
    vec4 eyePos;

    getEyeSpace( eyeNorm, eyePos);

    Colour = vec3 (0.0);
    for(int i=0; i<2; i++)
    {
        Colour += phongModel( i, eyeNorm, eyePos);
    }

    gl_Position = MVP*vec4(inVert,1.0);

    alpha = m_alpha;

}
