// CS148 Summer 2016 Homework 3 - Shaders

#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // TODO: Replace with your code...
    // If gl_Position was set correctly, this gives a totally red cube
    

	vec3 n = normalize(Normal);
	vec3 l = normalize(lightPos-FragPos); 
	vec3 e = normalize(viewPos-FragPos);
	vec3 h = (e+l) / length(e+l);
	
	
	vec3 result = (.1f+max(dot(n,l),0.0)+0.5f*pow(dot(h,n),64)) * lightColor * objectColor;
	color = vec4(result, 1.0f);
} 
