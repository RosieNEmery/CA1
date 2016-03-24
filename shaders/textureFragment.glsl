#version 410

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform float m_alpha;
uniform sampler2D texSmoke;
uniform sampler2D texAlpha;

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


uniform float MinPSize;
uniform float MaxPSize;


layout (location = 0) out vec4 fragmentColour;

vec3 phong( vec3 Position, vec3 Normal)
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
    vec3 ambAndDiff, spec;
    vec4 alphaMap = texture( texAlpha, TexCoord);

    /*if (alphaMap.a < 0.15)
        discard;
    else
    {
        if (gl_FrontFacing)
        {
            fragmentColour = vec4(phong(Position, Normal), 1.0);
       }
       else
        {
            fragmentColour = vec4(phong(Position, -Normal), 1.0);
        }
    }

*/

    fragmentColour = texture( texSmoke, TexCoord) * m_alpha;
    //fragmentColour = vec4(ambAndDiff, m_alpha) * texColor + vec4(spec, m_alpha);
  //  fragmentColour =baseColour;
}
