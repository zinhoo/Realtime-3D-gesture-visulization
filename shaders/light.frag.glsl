#version 330 core

uniform int myrenderStyle;

in vec2 mytexture;

uniform sampler2D tex;

void main (void)
{   

	gl_FragColor = texture2D(tex, mytexture.st);

}
