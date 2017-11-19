// Copyright (c), Tamas Csala

const char* debug_texture_vert_shader_string = R"""(

#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;

out vec2 vTexCoord;

void main() {
  vTexCoord = aTexCoord;
  gl_Position = vec4(aPosition, 0, 1);
}

)""";
