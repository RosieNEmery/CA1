#version 410 core

uniform sampler2D texSprite;
out vec4 fragColour;
void main ()
{
  fragColour = texture(texSprite,gl_PointCoord);
}
