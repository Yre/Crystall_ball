// CS148 Summer 2016 Homework 3 - Shaders

#version 330 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

vec3 myMult(vec3 a,vec3 b)
{
    return vec3(a.x*b.x,a.y*b.y,a.z*b.z);
}

void main()
{
    // TODO: Replace with your code...
    // If gl_Position was set correctly, this gives a totally red cube
    //color = vec4(vec3(1.f,0.f,0.f), 1.0f);
    vec3 e = normalize(viewPos-FragPos);
    vec3 l = normalize(lightPos-FragPos);
    vec3 h = normalize(e+l);
    float nl = dot(normalize(Normal),l);
    if(nl<0)
    	nl=0;
    vec3 c3 = myMult(objectColor, vec3(0.05f,0.05f,0.05f)+lightColor*nl) + lightColor*pow(nl,80);
    color = vec4(c3,1.0f);
} 


