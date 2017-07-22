#include "button.hpp"

const char *ButtonShader::VERTEX =
" attribute vec2 position; "
" attribute vec2 uv; "
" varying vec2 fragmentUV; "
" uniform mat4 projection; "
" uniform mat4 view; "
" void main() { "
"   fragmentUV = uv; "
"   gl_Position = projection * view * vec4(position, 0.0, 1.0); "
" } ";

const char *ButtonShader::FRAGMENT =
" precision lowp float; "
" varying vec2 fragmentUV; "
" uniform sampler2D texture; "
" uniform vec4 albedo; "
" void main() { "
"   vec4 color = texture2D(texture, fragmentUV); "
"   gl_FragColor = vec4( "
"      (color.r + albedo.r) * color.a, "
"      (color.g + albedo.g) * color.a, "
"      (color.b + albedo.b) * color.a, "
"      color.a "
"   ); "
" } ";
