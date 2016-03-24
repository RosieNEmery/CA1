#version 410

in vec3 Position;
in vec3 Normal;

in float alpha;


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
   // vec3 Ks;    //specular reflectivity
   // float Shininess; //specular shininess factor
};
uniform MaterialInfo Material;

uniform float m_alpha;

const int levels = 3;
//how many distinct values used in diffuse calculation (could be uniform)
const float scaleFactor = 1.0/levels;
out vec4 fragmentColour;

vec3 toonShader()
{

    vec3 s;

    //checking if light is directional or not

    if( Light.pos.w == 0)
        s = normalize(vec3(Light.pos));
        //directional light, used as direction towards light source
    else
        s = normalize(vec3(Light.pos)-Position);
        //not directional light,light pos treated as location in eye coords
    //direction towards light source, (negated because the viewer is at the origin)

    float cosine = max( 0.0, dot(s, Normal));
    //find cosine between s and normal, can't get negative number
    //so use max(0.0, dot()) instead of max(dot(), 0.0)

    vec3 diffuse =  Material.Kd * floor( cosine*levels) / levels;
    // value will always be between 0 and (levels -1),
    //then divide that by levels to get a value of levels between 1 and 0
    //eg. a level between 0 and 1

    return Light.intensity * (Material.Ka + diffuse);
}


void main()
{

    fragmentColour = vec4 (toonShader(),m_alpha);
}
