#include "standard.hpp"

const char *StandardShader::VERTEX =
" attribute vec3 position; "
" attribute vec2 uv; "
" varying vec4 fragmentPosition; "
" varying vec2 fragmentUV; "
" uniform mat4 projection; "
" uniform mat4 view; "
" void main() { "
"   fragmentUV = uv; "
"   fragmentPosition = view * vec4(position, 1.0); "
"   gl_Position = projection * fragmentPosition; "
" } ";

const char *StandardShader::FRAGMENT =
" precision lowp float; "
" varying vec4 fragmentPosition; "
" varying vec2 fragmentUV; "
" uniform sampler2D texture; "
" const float LOG2 = 1.442695; "
" const float density = 0.03; "
" const vec4 fog = vec4(0.04, 0.08, 0.14, 1.0); "
" void main() { "
"   float distance = length(fragmentPosition); "
"   float fogFactor = clamp(exp2(-density * density * distance * distance * LOG2), 0.0, 1.0); "
"   gl_FragColor = mix(fog, texture2D(texture, fragmentUV), fogFactor); "
" } ";
