#version 300 es

// fFlyEye.glsl
// Author: gre
// License: MIT

#ifdef GL_ES
// Set default precision to high
precision highp int;
precision highp float;
#endif

const float size = 0.04;
const float zoom = 50.0;
const float colorSeparation = 0.3;


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


vec4 transition(vec2 p) {
  float inv = 1. - progress;
  vec2 disp = size*vec2(cos(zoom*p.x), sin(zoom*p.y));
  vec4 texTo = getToColor(p + inv*disp);
  vec4 texFrom = vec4(
    getFromColor(p + progress*disp*(1.0 - colorSeparation)).r,
    getFromColor(p + progress*disp).g,
    getFromColor(p + progress*disp*(1.0 + colorSeparation)).b,
    1.0);
  return texTo*progress + texFrom*inv;
}


void
main(void) {
    glFragColor = transition(v_texcoord);
}
