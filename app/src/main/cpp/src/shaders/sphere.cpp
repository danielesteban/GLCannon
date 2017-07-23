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
" const float LOG2 = 1.442695; "
" const float density = 0.03; "
" const vec4 fog = vec4(0.8, 0.658, 0.407, 1.0); "
" const float ambientFactor = 0.15; "
" const vec3 lightPosition = vec3(-0.1, 0.9, -1.0); "
" void main() { "
"   float distance = length(fragmentPosition); "
"   float fogFactor = clamp(exp2(-density * density * distance * distance * LOG2), 0.0, 1.0); "
"   float lightFactor = max(dot(normalize(fragmentNormal), normalize(lightPosition)), ambientFactor);  "
"   gl_FragColor = mix(fog, albedo * lightFactor, fogFactor); "
" } ";
