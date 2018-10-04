#version 430

out vec4 Color;
in vec4 theColor;

void main()
{
	Color = vec4(theColor);
};