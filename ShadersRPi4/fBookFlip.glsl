#version 300 es

// Author: hong
// License: MIT

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


vec2
skewRight(vec2 p) {
  float skewX = (p.x - progress)/(0.5 - progress) * 0.5;
  float skewY =  (p.y - 0.5)/(0.5 + progress * (p.x - 0.5) / 0.5)* 0.5  + 0.5;
  return vec2(skewX, skewY);
}

vec2
skewLeft(vec2 p) {
  float skewX = (p.x - 0.5)/(progress - 0.5) * 0.5 + 0.5;
  float skewY = (p.y - 0.5) / (0.5 + (1.0 - progress ) * (0.5 - p.x) / 0.5) * 0.5  + 0.5;
  return vec2(skewX, skewY);
}

vec4
addShade() {
  float shadeVal  =  max(0.7, abs(progress - 0.5) * 2.0);
  return vec4(vec3(shadeVal ), 1.0);
}

vec4
transition (vec2 p) {
  float pr = step(1.0 - progress, p.x);
  if (p.x < 0.5) {
    return mix(getFromColor(p), getToColor(skewLeft(p)) * addShade(), pr);
  } else {
    return mix(getFromColor(skewRight(p)) * addShade(), getToColor(p),   pr);
  }
}


void
main(void) {
    glFragColor = transition(v_texcoord);
}
