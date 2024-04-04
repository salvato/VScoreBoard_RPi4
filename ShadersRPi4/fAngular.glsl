#version 300 es

// Author: Fernando Kuteken
// License: MIT

#ifdef GL_ES
// Set default precision to high
precision highp int;
precision highp float;
#endif

const float PI = 3.141592653589;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float progress;
const float startingAngle=90.0;

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


vec4
transition (vec2 uv) {
  float offset = startingAngle * PI / 180.0;
  float angle = atan(uv.y - 0.5, uv.x - 0.5) + offset;
  float normalizedAngle = (angle + PI) / (2.0 * PI);
  
  normalizedAngle = normalizedAngle - floor(normalizedAngle);
  return mix(getFromColor(uv),
             getToColor(uv),
             step(normalizedAngle, progress));
}


void
main(void) {
    glFragColor = transition(v_texcoord);
}
