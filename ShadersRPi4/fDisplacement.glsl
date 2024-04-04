#version 300 es

// fDisplacement.glsl
// Author: Travis Fischer
// License: MIT
//
// Adapted from a Codrops article by Robin Delaporte
// https://tympanus.net/Development/DistortionHoverEffect


#ifdef GL_ES
// Set default precision to high
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


uniform sampler2D displacementMap;

const float strength = 0.5;

vec4 transition (vec2 uv) {
  float displacement = texture2D(displacementMap, uv).r * strength;

  vec2 uvFrom = vec2(uv.x + progress * displacement, uv.y);
  vec2 uvTo = vec2(uv.x - (1.0 - progress) * displacement, uv.y);

  return mix(
    getFromColor(uvFrom),
    getToColor(uvTo),
    progress
  );
}

void
main(void) {
    glFragColor = transition(v_texcoord);
}
