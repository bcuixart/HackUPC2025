#version 330 core

in vec3 fcolor;
in vec3 n_vec;

uniform vec3 l_pos;
uniform vec3 l_col;
uniform vec3 amb_l_col;

out vec4 FragColor;

void main() {
    FragColor = vec4(fcolor, 1);
}

