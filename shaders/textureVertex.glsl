#version 410

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inNormal;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 MV;
uniform mat3 normalMatrix;
uniform mat4 MVP;


void main()
{
    TexCoord = inUV;

    Normal = normalize( normalMatrix * inNormal);
    inVert.y = (inVert.y*-1);
    Position = vec3(MV * vec4(inVert.xyz,1.0));

    gl_Position = MVP*vec4(inVert,1.0);

}
