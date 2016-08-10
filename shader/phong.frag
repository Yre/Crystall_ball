

#version 330 core
//in vec3 Normal;
//in vec3 Position;
////in vec2 TexCoords;
//
//uniform vec3 viewPos;
////uniform sampler2D texture_diffuse1;
////uniform sampler2D texture_reflection1;
//uniform samplerCube skybox;
//
//out vec4 color;
//
//void main()
//{
//    /*
//    // Diffuse
//    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);
//    // Reflection
//    vec3 I = normalize(Position - cameraPos);
//    vec3 R = reflect(I, normalize(Normal));
//    float reflect_intensity = texture(texture_reflection1, TexCoords).r;
//    vec4 reflect_color;
//    if(reflect_intensity > 0.1) // Only sample reflections when above a certain treshold
//        reflect_color = texture(skybox, R) * reflect_intensity;
//    // Combine them
//    color = diffuse_color + reflect_color;
//     */
//
//
//
//    /*
//    float ratio = 1.00 / 1.5;
//    vec3 I = normalize(Position - viewPos);
//    vec3 R = refract(I, normalize(Normal), ratio);
//    color = texture(skybox, R);
//     */
//
//
//
//    /*
//    const float etaR = 1.14;
//    const float etaG = 1.12;
//    const float etaB = 1.10;
//    const float fresnelPower = 2.0;
//    const float F = ((1.0 - etaG) * (1.0 - etaG)) / ((1.0 + etaG) * (1.0 + etaG));
//    vec3 i = normalize(Position);
//    vec3 n = normalize(Normal);
//
//    float ratio = F + (1.0 - F) * pow(1.0 - dot(-i, n), fresnelPower);
//
//    vec3 refractR = vec3(gl_TextureMatrix[0] * vec4(refract(i, n, etaR), 1.0));
//    vec3 refractG = vec3(gl_TextureMatrix[0] * vec4(refract(i, n, etaG), 1.0));
//    vec3 refractB = vec3(gl_TextureMatrix[0] * vec4(refract(i, n, etaB), 1.0));
//
//    vec3 reflectDir = vec3(gl_TextureMatrix[0] * vec4(reflect(i, n), 1.0));
//
//    vec4 refractColor;
//    refractColor.ra = textureCube(environment, refractR).ra;
//    refractColor.g  = textureCube(environment, refractG).g;
//    refractColor.b  = textureCube(environment, refractB).b;
//
//    vec4 reflectColor;
//    reflectColor    = textureCube(environment, reflectDir);
//
//    vec3 combinedColor = mix(refractColor, reflectColor, ratio);
//
//    gl_FragColor = vec4(combinedColor, 1.0);
//     */
//
//}

in vec3 v_refraction;
in vec3 v_reflection;
in float v_fresnel;
out vec4 color;
uniform samplerCube skybox;

void main(void)
{
    vec4 refractionColor = texture( skybox, normalize( v_refraction ) );
    vec4 reflectionColor = texture( skybox, normalize( v_reflection ) );
    
    color = mix( refractionColor, reflectionColor, v_fresnel );
}
