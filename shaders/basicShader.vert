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
    fcolor = color;
    fpos = (TG * vec4(vertex, 1)).xyz;

    if (doesWaves == 1) 
    {
        float angle = atan(vertex.z, vertex.x);

        float n = noise(angle);
        vec3 variation = vec3(n * 0.1);  // petita variació (±10%)
        fcolor = clamp(waveColor + variation, 0.0, 1.0);

        float offset = random(angle) * .5;
        float wave = sin(angle * waveFrequency - time * waveSpeed + offset);
      
        gl_Position = PM * VM * TG * vec4 (vertex.x, vertex.y + wave * waveAmplitude, vertex.z, 1.0);

    } else {
        gl_Position = PM * VM * TG * vec4 (vertex, 1.0);
    }

}
