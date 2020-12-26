#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 tex_coord;

out vec2 out_tex_coord;
out vec4 out_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.f);
    out_tex_coord = tex_coord;
    out_color = color;
}
