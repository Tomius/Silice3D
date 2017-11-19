// Copyright (c), Tamas Csala

const char* mesh_frag_shader_string = R"""(

#version 330 core

#include "Silice3D/lighting.frag"
#include "Silice3D/post_process.frag"
#include "Silice3D/bicubic_sampling.glsl"

in vec3 w_vPos;
in vec3 w_vNormal;
in vec2 vTexCoord;

out vec4 fragColor;

uniform sampler2D uDiffuseTexture;

void main() {
  vec3 diffuse_color = texture(uDiffuseTexture, vTexCoord).rgb;
  vec3 output_color = Silice3D_CalculateLighting(w_vPos, normalize(w_vNormal), false,
                                                 diffuse_color, diffuse_color, 16);
  fragColor = vec4(PostProcess(output_color), 1.0);
}

)""";
