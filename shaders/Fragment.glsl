#version 410 core

uniform sampler2D texSmoke;

in vec2 vertUV;

in vec3 Normals;
flat in float Alpha;

layout (location=0)out vec4 FragmentColour;
void main ()
{
 //normals not working!!!

 FragmentColour = vec4(Normals, 1.0);

 //FragmentColour = texture(texSmoke,vertUV);//* (Alpha);
}
