#version 330 core
in vec3 Position;
in vec3 Color;
out vec4 color;
uniform vec3 center;
uniform float sideLength;

  
void main()
{

//a: 1/2 ~ sqrt(2)/2 * length  -> 0~sqrt(2)-1: 0~0.4... -> -0.4~0
	float a = (length(Position - center))/sideLength * 2 - 1;
	float myAlpha = exp(-a*10);
	color = vec4(Color, myAlpha);
} 
