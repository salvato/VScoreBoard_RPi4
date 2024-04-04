#version 400


in vec3 vPosition;
in vec3 vNormal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;


uniform mat4 camera;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;


void
main() {
    vs_out.FragPos = vec3(model * vec4(vPosition, 1.0));
    vs_out.Normal = transpose(inverse(mat3(model))) * vNormal;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = view * camera * model * vec4(vPosition, 1.0);
}
