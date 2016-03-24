#version 410

layout (location = 0) in vec3 inVert;
layout (location = 2) in vec3 inNormal;

out vec3 Position;
out vec3 Normal;

uniform mat4 MV;
uniform mat3 normalMatrix;
uniform mat4 MVP;

void getEyeSpace(out vec3 norm, out vec4 pos)
{
    Normal = normalize( normalMatrix * inNormal);
    Position = vec3(MV * vec4(inVert,1.0));
}

void main()
{

    vec3 eyeNorm;
    vec4 eyePos;

    getEyeSpace( eyeNorm, eyePos);

    gl_Position = MVP*vec4(inVert,1.0);

}
