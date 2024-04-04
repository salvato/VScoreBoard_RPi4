#version 300 es

// fCrosswarp.glsl
// Author: Eke Péter <peterekepeter@gmail.com>
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



vec4 transition(vec2 p) {
  float x = progress;
  x=smoothstep(.0,1.0,(x*2.0+p.x-1.0));
  return mix(getFromColor((p-.5)*(1.-x)+.5), getToColor((p-.5)*x+.5), x);
}


void
main(void) {
    glFragColor = transition(v_texcoord);
}
