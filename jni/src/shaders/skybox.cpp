#include "skybox.hpp"

const char *SkyboxShader::VERTEX =
" attribute vec3 position; "
" varying vec3 fragmentPosition; "
" uniform mat4 projection; "
" uniform mat4 view; "
" void main() { "
"   fragmentPosition = position; "
"   vec4 pos = projection * view * vec4(position, 1.0); "
"   gl_Position = pos.xyww; "
" } ";

const char *SkyboxShader::FRAGMENT =
" precision lowp float; "
" varying vec3 fragmentPosition; "
" uniform samplerCube texture; "
" void main() { "
"   gl_FragColor = textureCube(texture, fragmentPosition); "
" } ";
