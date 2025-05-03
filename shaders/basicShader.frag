#version 330 core

in vec3 pos;
in vec3 n_vec;

in vec3 amb;
in vec3 diff;
in vec3 spec;
in float shin;

in vec3 fs_l_pos;

uniform vec3 obs;
uniform vec3 l_col;
uniform vec3 amb_l_col;

out vec4 FragColor;

vec3 calc_amb() {
    return amb_l_col*amb;
}

vec3 calc_diff() {
    vec3 L = normalize(fs_l_pos - pos);
    return (diff*l_col*max(dot(n_vec, L), 0.0));
}

vec3 calc_spec() {
    vec3 v = normalize(obs - pos);
    vec3 L = fs_l_pos - pos;
    vec3 R = normalize(2*n_vec*dot(n_vec, L) - L);
    
    if (dot(normalize(n_vec), normalize(L)) < 0) {
        return l_col*spec*pow(dot(R,v), 100);
    }
    else return vec3(0.0,0.0,0.0);
}

void main() {
    FragColor = vec4(calc_diff() + calc_amb() + calc_spec(), 1.0);
}

