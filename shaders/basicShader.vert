#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform vec3 l_pos;
uniform vec3 l_col;
uniform vec3 amb_l_col;

uniform mat4 TG;
uniform mat4 PM;
uniform mat4 VM;

out vec3 fcolor;
out n_vec;

void main()  {
    fcolor = matdiff;
    gl_Position = PM * VM * TG * vec4 (vertex, 1.0);
}
