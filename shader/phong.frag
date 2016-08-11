#version 330 core


in vec3 v_refraction;
in vec3 v_reflection;
in float v_fresnel;
in vec3 FragPos;  
in vec3 Normal; 
out vec4 color;
uniform samplerCube skybox;


uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void)
{

	vec3 n = normalize(Normal);
	vec3 l = normalize(lightPos-FragPos); 
	vec3 e = normalize(viewPos-FragPos);
	vec3 h = (e+l) / length(e+l);
	


    vec4 refractionColor = texture( skybox, normalize( v_refraction ) );
    vec4 reflectionColor = texture( skybox, normalize( v_reflection ) );
    
    color = mix( refractionColor, reflectionColor, v_fresnel );

    vec3 result = (color.xyz + 0.8*max(dot(n,l),0.0)+0.5f*pow(dot(h,n),64)) * lightColor * objectColor;

    color = mix(color, vec4(result, 1.0f), 0.4);
}
