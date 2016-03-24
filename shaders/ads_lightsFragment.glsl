#version 410

in vec3 Colour;
in float alpha;

out vec4 fragmentColour;

void main()
{
    /* for making it discard parts of mesh
   const float scale = 1.0;

   bvec2 toDiscard = greaterThan( fract(TexCoord * scale), vec2(0.2,0.2));

    if( all(toDiscard))
        discard;
   */

    fragmentColour = vec4 (Colour,alpha);
}
