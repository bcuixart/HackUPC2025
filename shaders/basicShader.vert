#version 330 core

in vec3 vertex;
in vec3 color;

uniform mat4 TG;
uniform mat4 PM;
uniform mat4 VM;

out vec3 fpos;
out vec3 fcolor;

uniform int doesWaves;

uniform float time;
uniform float waveAmplitude = 1;
uniform float waveFrequency = 20.0;
uniform float waveSpeed = 1.0;

void main()  {
    fcolor = color;
    fpos = vertex;

    if (doesWaves == 1) {
        float angle = atan(vertex.z, vertex.x);
        float wave = sin(angle * waveFrequency - time * waveSpeed);
      
        gl_Position = PM * VM * TG * vec4 (vertex.x, vertex.y + wave * waveAmplitude, vertex.z, 1.0);

    } else {
        gl_Position = PM * VM * TG * vec4 (vertex, 1.0);
    }

}
