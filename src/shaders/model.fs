#version 460 core

in vec2 out_tex_coord;
in vec4 out_color;

out vec4 frag_color;

uniform sampler2D sampler0;
uniform int textured;

void main() {
    if (textured > 0) {
        frag_color = texture(sampler0, out_tex_coord);
    }
    else {
        frag_color = out_color;
    }
}
