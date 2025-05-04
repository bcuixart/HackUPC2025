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

out vec3 fpos;
out vec3 n_vec;

uniform vec3 l_pos;
out vec3 fs_l_pos;

out vec3 famb;
out vec3 fdiff;
out vec3 fspec;
out float fshin;

uniform int doesWaves;
uniform int getsLighting;
flat out int fgetsLighting;

uniform float time;
uniform float waveAmplitude = .5;
uniform float waveFrequency = 10.0;
uniform float waveSpeed = 1.0;
uniform vec3 waveColor;

float random(float x) {
    return fract(sin(x) * 43758.5453123);
}

float noise(float x) {
    return sin(x * 12.9898 + time * 0.5) * 0.5 + 0.5;
}


void main()  {
    fgetsLighting = getsLighting;
    fdiff = matdiff;
    fpos = (TG * vec4(vertex, 1)).xyz;

    if (doesWaves == 1) 
    {
        float angle = atan(vertex.z, vertex.x);

        float n = noise(angle);
        vec3 variation = vec3(n * 0.1);
        fdiff = clamp(waveColor + variation, 0.0, 1.0);

        float offset = random(angle) * .5;
        float wave = sin(angle * waveFrequency - time * waveSpeed + offset);
      
        gl_Position = PM * VM * TG * vec4 (vertex.x, vertex.y + wave * waveAmplitude, vertex.z, 1.0);

        return;
    } 

    if (getsLighting == 1) {
        n_vec = normal*inverse(transpose(mat3(VM*TG)));
        n_vec = normalize(n_vec);
    
        famb = matamb;
        fdiff = matdiff;
        fspec = matspec;
        fshin = matshin;
        fpos = gl_Position.xyz;

        fs_l_pos = (transpose(VM)*vec4(l_pos,1.0)).xyz;
    }
    
    gl_Position = PM * VM * TG * vec4 (vertex, 1.0);

}