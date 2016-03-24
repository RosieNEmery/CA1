#version 410

in vec3 lightIntensity;
in vec3 test;
in float alpha;

out vec4 fragmentColour;

void main()
{
    fragmentColour = vec4 (lightIntensity, alpha);
}
