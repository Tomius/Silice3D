// Copyright (c), Tamas Csala

const char* debug_shape_frag_shader_string = R"""(

#version 330 core

in vec3 w_vNormal;

uniform vec3 uColor = vec3(1.0);

out vec4 fragColor;

void main() {
  vec3 fake_light_pos = normalize(vec3(0.4, 0.8, 0.2));
  float dot_value = dot(normalize(w_vNormal), fake_light_pos);
  vec3 fake_lighting = mix(vec3(0.09), vec3(0.9), (1 + dot_value)/2);
  fragColor = vec4(uColor * fake_lighting, 1);
}

)""";
