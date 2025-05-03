#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3 matamb;
in vec3 matdiff;
in vec3 matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 PM;
uniform mat4 VM;

uniform vec3 l_pos;
out vec3 fs_l_pos;

out vec3 pos;
out vec3 n_vec;

out vec3 amb;
out vec3 diff;
out vec3 spec;
out float shin;

void main()  {


    n_vec = normal*inverse(transpose(mat3(VM*TG)));
    n_vec = normalize(n_vec);
    gl_Position = PM * VM * TG * vec4 (vertex, 1.0);
    
    amb = matamb;
    diff = matdiff;
    spec = matspec;
    shin = matshin;
    pos = gl_Position.xyz;


    fs_l_pos = (transpose(VM)*vec4(l_pos,1.0)).xyz;
    
}
