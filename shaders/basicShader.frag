#version 330 core

in vec3 fpos;
in vec3 fcolor;

out vec4 FragColor;

uniform float fadeDistance = 7;

uniform int doFog = 0;

void main() {
    vec3 bColor = vec3(0.105, 0.058, 0.5);

    if (doFog == 1) {
        float dist = length(fpos);
        float t = clamp(dist / fadeDistance, 0.0, 1.0);
        vec3 finalColor = mix(fcolor, bColor, t);
        FragColor = vec4(finalColor, 1.0);
    } else {
        FragColor = vec4(fcolor, 1.0);
    }
}

