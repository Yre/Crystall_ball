#version 330 core


in vec3 v_refraction;
in vec3 v_reflection;
in float v_fresnel;
in vec3 FragPos;  
in vec3 Normal;
in vec3 incident;
out vec4 color;
in float deep;
uniform samplerCube skybox;


uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

const float etaR = 1.14;
const float etaG = 1.12;
const float etaB = 1.10;
const float F = ((1.0 - etaG) * (1.0 - etaG)) / ((1.0 + etaG) * (1.0 + etaG));

void main(void)
{

	vec3 n = normalize(Normal);
	vec3 l = normalize(lightPos-FragPos); 
	vec3 e = normalize(viewPos-FragPos);
	vec3 h = (e+l) / length(e+l);
    
	//
    vec3 i = incident;
    //float ratio = F + (1.0 - F) * pow(1.0 - dot(-i, n), 2);
    vec3 refractR = vec3(vec4(refract(i, n, etaR), 1.0));
    vec3 refractG = vec3(vec4(refract(i, n, etaG), 1.0));
    vec3 refractB = vec3(vec4(refract(i, n, etaB), 1.0));
    vec3 reflectDir = vec3(vec4(reflect(i, n), 1.0));
    
    vec4 refractColor;
    refractColor.ra = texture(skybox, refractR).ra;
    refractColor.g  = texture(skybox, refractG).g;
    refractColor.b  = texture(skybox, refractB).b;
    
    vec4 reflectColor;
    reflectColor    = texture(skybox, reflectDir);
    
    color = mix(refractColor*0, reflectColor, 1);
    //return ;
    
    /*

    vec4 refractionColor = texture( skybox, normalize( v_refraction ) );
    vec4 reflectionColor = texture( skybox, normalize( v_reflection ) );
    
    color = mix( refractionColor, reflectionColor, v_fresnel );
    
    //return;
    */
    
    vec3 result = (0.3+0.2*max(dot(n,l),0.0)+0.3f*pow(dot(h,n),64)) * lightColor * objectColor;
    color = color + vec4(result*0.3, 1.0f);
    if(deep>0.4)
        color.a=0.0f;
}
