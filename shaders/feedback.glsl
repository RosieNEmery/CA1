#version 410 core

uniform mat4 View;

layout (location =0)in vec4 inPos;

//passed out to TexShader, need to get Lifetime out as well
out mat4 ModelView;
//out float Lifetime;


void main()
{
    //billboards are smaller when they start dying
    float size = (inPos.w)*0.05;
    mat4 Model = mat4(   size, 0.0, 0.0, 0.0,
                         0.0, size, 0.0, 0.0,
                         0.0, 0.0, size, 0.0,
                         0.0, 0.0, 0.0, 1.0);
    Model[3].xyz = inPos.xyz;

    //set Lifetime to lifetime of particle passed in
    //Lifetime = inPos.w;

    ModelView = View * Model;
}
