#version 300 es

in vec3 vPosition;
in vec2 vTexture;

out vec2 TexCoords;

void
main() {
    TexCoords = vTexture;
    gl_Position = vec4(vPosition, 1.0);
}
