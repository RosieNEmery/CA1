#version 410

layout (location = 0) in vec3 inVert;
layout (location = 2) in vec3 inNormal;

out vec3 lightIntensity;
out vec3 test;
out float alpha;


uniform vec4 lightPos;  //light pos
uniform vec3 Kd;             //object col
uniform vec3 Ld;
uniform float m_alpha; //light col

uniform mat4 MV;
uniform mat3 normalMatrix;
uniform mat4 MVP;

void main()
{

    vec3 tnorm = normalize( normalMatrix * inNormal);
    vec4 eyeCoords = MV * vec4(inVert,1.0);
    vec3 s = normalize(vec3(lightPos - eyeCoords));


    test = Ld*Kd*max(dot(s,tnorm), 0.0);

    lightIntensity = Ld*Kd*max(dot(s,tnorm), 0.0);
    gl_Position = MVP*vec4(inVert,1.0);

    alpha = m_alpha;

}
