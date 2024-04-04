#version 300 es

// fRipple.glsl
// Author: gre
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


const float amplitude = 100.0;
const float speed = 50.0;

vec4 transition (vec2 uv) {
    vec2 dir = uv - vec2(.5);
    float dist = length(dir);
    vec2 offset = dir * (sin(progress * dist * amplitude - progress * speed) + .5) / 30.;
    return mix(
                getFromColor(uv + offset),
                getToColor(uv),
                smoothstep(0.2, 1.0, progress)
                );
}

void
main(void) {
    glFragColor = transition(v_texcoord);
}
