#version 300 es

// fRadial.glsl
// License: MIT
// Author: Xaychru
// ported by gre from https://gist.github.com/Xaychru/ce1d48f0ce00bb379750


#ifdef GL_ES
// Set default precision to medium
precision highp int;
precision highp float;
#endif


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


const float smoothness = 1.0;
const float PI = 3.141592653589;

vec4 transition(vec2 p) {
  vec2 rp = p*2.-1.;
  return mix(
    getToColor(p),
    getFromColor(p),
    smoothstep(0., smoothness, atan(rp.y,rp.x) - (progress-.5) * PI * 2.5)
  );
}


void
main(void) {
    glFragColor = transition(v_texcoord);
}
