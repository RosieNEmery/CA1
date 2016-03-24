#version 410


in vec3 Position;
in vec3 Normal;

struct LightInfo
{
    vec4 pos;  //light pos
    vec3 intensity;   //intensity of light

};
uniform LightInfo Light;

//for FOG/////////////////////////////////////////////
struct FogInfo
{
    float maxDis; //distance from eye where fog = 100%
    float minDis; //distance from eye where fog = 0%
    vec3 colour; //colour of fog
};
uniform FogInfo Fog;
/////////////////////////////////////////////////////

struct MaterialInfo
{
    vec3 Ka;    //Ambient reflectivity
    vec3 Kd;    //diffuse reflectivity
    vec3 Ks;    //specular reflectivity
    float Shininess; //specular shininess factor
};

uniform MaterialInfo Material;

uniform float m_alpha;

out vec4 fragmentColour;

vec3 phong()
{
    vec3 n = normalize( Normal);

    vec3 s;

    //checking if light is directional or not

    if( Light.pos.w == 0)
        s = normalize(vec3(Light.pos));
        //directional light, used as direction towards light source
    else
        s = normalize(vec3(Light.pos)-Position);
        //not directional light,light pos treated as location in eye coords
    //direction towards light source, (negated because the viewer is at the origin)

    vec3 v = normalize(-Position.xyz); //swizzling

    //calculate half vector
    vec3 h = normalize( v + s );

    //compute pure reflection
    //vec3 r = reflect(-s, n);

    //compute diffuse
    vec3 diffuse =  Material.Kd * max(dot(s,n), 0.0);

    //compute specular
    vec3 spec =  Material.Ks * pow(max(dot(h,n),0.0), Material.Shininess);

    //Is = LsKs(r.v)^f

    return Light.intensity * (Material.Ka + diffuse + spec);
}


void main()
{
    //for FOG/////////////////////////////////////////////

    float dist = length ( Position.xyz);
    //distance from surface point to eye position, z position used to estimate distance from eye
    float fogFactor = (Fog.maxDis - dist) / (Fog.maxDis - Fog.minDis);
    //fog factor = equation that calculates how much fog is at point dist
    fogFactor = clamp (fogFactor, 0.0, 1.0);
    //clamps value between 0 (no fog) and 1 (100% fog) - if dist is not in range of
    //max.dis or min.dos
    vec3 shadeColour = phong();

    vec3 colour = mix(Fog.colour, shadeColour, fogFactor);
    //mix colours according to fogFactor value

    /////////////////////////////////////////////////////////////////////

    fragmentColour = vec4 (colour,m_alpha);
}
