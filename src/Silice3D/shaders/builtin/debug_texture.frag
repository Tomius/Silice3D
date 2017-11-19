// Copyright (c), Tamas Csala

const char* debug_texture_frag_shader_string = R"""(

#version 330 core

in vec2 vTexCoord;

uniform sampler2D uTex;

out vec4 fragColor;

void main() {
  fragColor = vec4(texture2D(uTex, vec2(vTexCoord.x, 1-vTexCoord.y)).rgb, 1);
}

)""";
