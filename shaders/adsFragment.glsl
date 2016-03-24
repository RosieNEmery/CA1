#version 410

in vec3 lightIntensity;
in vec3 test;
in float alpha;

in vec2 TexCoord;

out vec4 fragmentColour;

void main()
{
    /* for making it discard parts of mesh
   const float scale = 1.0;

   bvec2 toDiscard = greaterThan( fract(TexCoord * scale), vec2(0.2,0.2));

    if( all(toDiscard))
        discard;
   */

    fragmentColour = vec4 (lightIntensity,alpha);
}
