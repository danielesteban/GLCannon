#include "button.hpp"

const char *ButtonShader::VERTEX =
" attribute vec2 position; "
" uniform mat4 projection; "
" uniform mat4 view; "
" void main() { "
"   gl_Position = projection * view * vec4(position, 0.0, 1.0); "
" } ";

const char *ButtonShader::FRAGMENT =
" precision lowp float; "
" uniform vec4 albedo; "
" void main() { "
"   gl_FragColor = vec4(albedo.r * albedo.a, albedo.g * albedo.a, albedo.b * albedo.a, albedo.a); "
" } ";
