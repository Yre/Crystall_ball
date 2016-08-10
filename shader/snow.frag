#version 330 core
in vec3 Position;
out vec4 color;
uniform vec3 center;
uniform float 
  
void main()
{

	float a = length(Position - center);
	float myAlpha = exp(-a*10);
	color = vec4(1.0f, 0.0f, 0.0f, myAlpha);
} 
