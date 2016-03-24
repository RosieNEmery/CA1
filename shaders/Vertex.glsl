#version 410 core

uniform mat4 Projection;
uniform mat4 View;

//uniform float Timer;
uniform float maxLife;

layout (location =0) in vec3 inVert;
layout(location =1) in vec2 inUV;
layout(location =3) in mat4 inModelView;

//uses normals from billboardvao
layout(location =2) in vec3 inNormal;

//needs to use transform feedback, look into GL_INTERLEAVED_ATTRIB more
//layout(location =7) in float Lifetime;

out vec2 vertUV;
out vec3 Normals;

//sends out Alpha without interpolation across fragment
flat out float Alpha;


void main()
{
        mat4 ModelView = inModelView;
        mat4 ModelViewProjection = Projection * ModelView;
	gl_Position = ModelViewProjection*vec4(inVert, 1.0);

        //Normals use Model matrix?
        mat4 Model = ModelView/View;
        //doesn't work :'(
        Normals = (Model*vec4(inNormal,0.0)).xyz;

        vertUV=inUV.st;

}
