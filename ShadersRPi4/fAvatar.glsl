#version 300 es


#ifdef GL_ES
// Set default precision to high
precision highp int;
precision highp float;
#endif

uniform sampler2D texture;
out vec2 v_texcoord;


void
main() {
    // Set fragment color from texture
    gl_FragColor = texture2D(texture, v_texcoord);
}

