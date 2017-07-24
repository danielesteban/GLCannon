#include "menu.hpp"

const char *MenuShader::VERTEX =
" attribute vec2 position; "
" attribute vec2 uv; "
" attribute float item; "
" varying vec2 fragmentUV; "
" varying float fragmentItem; "
" uniform mat4 projection; "
" uniform mat4 view; "
" void main() { "
"   fragmentUV = uv; "
"   fragmentItem = item; "
"   gl_Position = projection * view * vec4(position, 0.0, 1.0); "
" } ";

const char *MenuShader::FRAGMENT =
" precision lowp float; "
" varying vec2 fragmentUV; "
" varying float fragmentItem; "
" uniform sampler2D texture; "
" uniform float active; "
" void main() { "
"   vec3 albedo = vec3(0.0, 0.0, 0.0); "
"   if (fragmentItem == active) { "
"     albedo.g = 1.0; "
"   } "
"   if (fragmentItem == 5.0) { "
"     albedo.b = 1.0; "
"   } "
"   vec4 color = texture2D(texture, fragmentUV); "
"   gl_FragColor = vec4(albedo * 0.9 + color.rgb * color.a, 0.8); "
" } ";
