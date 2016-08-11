#version 330 core


// Indices of refraction
const float Air = 1.0;
const float Glass = 4; //1.51714;//4

// Air to glass ratio of the indices of refraction (Eta)
const float Eta = Air / Glass;

// see http://en.wikipedia.org/wiki/Refractive_index Reflectivity
const float R0 = ((Air - Glass) * (Air - Glass)) / ((Air + Glass) * (Air + Glass));

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 viewPos;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 v_reflection;
out vec3 v_refraction;
out float v_fresnel;
out vec3 FragPos;
out vec3 Normal;
out vec3 incident;
out float deep;


void main(void){
    
    vec4 vertex = model * vec4( position, 1.0 );
    vec4 camera = vec4( viewPos, 1.0 );
    
    incident = normalize( vec3( vertex - camera ) );
    
    
    vec3 norm = mat3(transpose(inverse(model))) * normal;
    
    v_refraction = refract( incident, norm, Eta );
    v_reflection = reflect( incident, norm );
    
    v_fresnel = R0 + (1.0 - R0) * pow( (1.0 - dot( -incident, normal ) ), 5.0);
    
    gl_Position = projection * view * vertex;
    Normal = normal;
    FragPos = gl_Position.xyz/gl_Position.w;
    deep = position.y;
}
