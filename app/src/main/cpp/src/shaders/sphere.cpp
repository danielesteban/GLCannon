#include "sphere.hpp"

const char *SphereShader::VERTEX =
" attribute vec3 position; "
" attribute vec3 normal; "
" varying vec4 fragmentPosition; "
" varying vec3 fragmentNormal; "
" uniform mat4 projection; "
" uniform mat4 view; "
" uniform mat3 normalView; "
" void main() { "
"   fragmentNormal = normalView * normal; "
"   fragmentPosition = view * vec4(position, 1.0); "
"   gl_Position = projection * fragmentPosition; "
" } ";

const char *SphereShader::FRAGMENT =
" precision lowp float; "
" varying vec4 fragmentPosition; "
" varying vec3 fragmentNormal; "
" uniform vec4 albedo; "
" uniform vec4 fogColor; "
" uniform vec3 sunPosition; "
" const float LOG2 = 1.442695; "
" const float density = 0.03; "
" const float ambientFactor = 0.2; "
" void main() { "
"   float distance = length(fragmentPosition); "
"   float fogFactor = clamp(exp2(-density * density * distance * distance * LOG2), 0.0, 1.0); "
"   float lightFactor = max(dot(normalize(fragmentNormal), normalize(sunPosition)), ambientFactor);  "
"   gl_FragColor = mix(fogColor, albedo * lightFactor, fogFactor); "
" } ";
