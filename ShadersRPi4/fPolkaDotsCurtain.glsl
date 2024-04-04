#version 300 es

// fPolkDotsCurtain.glsl
// author: bobylito
// license: MIT

#ifdef GL_ES
// Set default precision to high
precision highp int;
precision highp float;
#endif

const float SQRT_2 = 1.414213562373;
const float dots = 20.0;
const vec2 center = vec2(0, 0);


uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float progress;
in vec2 v_texcoord;
out vec4 glFragColor;


vec4
getFromColor(vec2 p) {
    vec4 tempc;
    tempc = texture2D(texture0, vec2(p.x, p.y));
    return tempc;
}

vec4
getToColor(vec2 p) {
    vec4 tempc;
    tempc = texture2D(texture1, vec2(p.x, p.y));
    return tempc;
}


vec4 transition(vec2 uv) {
  bool nextImage = distance(fract(uv * dots), vec2(0.5, 0.5)) < ( progress / distance(uv, center));
  return nextImage ? getToColor(uv) : getFromColor(uv);
}


void
main(void) {
    glFragColor = transition(v_texcoord);
}
