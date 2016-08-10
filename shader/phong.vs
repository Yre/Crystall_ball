#version 330 core
//layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
////layout (location = 2) in vec2 texCoords;
//
//out vec3 Normal;
//out vec3 Position;
////out vec2 TexCoords;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
//uniform vec3 viewPos;
//
//void main()
//{
//
//    gl_Position = projection * view * model * vec4(position, 1.0f);
//    Normal = mat3(transpose(inverse(model))) * normal;
//    Position = vec3(model * vec4(position, 1.0f));
//    //TexCoords = vec2(position);
//
//    /*return;
//
//    vec4 p = projection * view * model * gl_Vertex;
//    Position = p.xyz / p.w - viewPos;
//    Normal   = mat3(transpose(inverse(model))) * normal;
//
//    gl_Position = ftransform();
//     */
//}


// Indices of refraction
const float Air = 1.0;
const float Glass = 4;//1.51714;

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


void main(void)
{
    // We calculate in world space.
    
    vec4 vertex = model * vec4( position, 1.0 );
    vec4 camera = vec4( viewPos, 1.0 );
    
    vec3 incident = normalize( vec3( vertex - camera ) );
    
    // Assume incoming normal is normalized.
    
    vec3 norm = mat3(transpose(inverse(model))) * normal;
    
    v_refraction = refract( incident, norm, Eta );
    v_reflection = reflect( incident, norm );
    
    // see http://en.wikipedia.org/wiki/Schlick%27s_approximation
    v_fresnel = R0 + (1.0 - R0) * pow( (1.0 - dot( -incident, normal ) ), 5.0);
    
    gl_Position = projection * view * vertex;
}