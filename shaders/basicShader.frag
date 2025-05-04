#version 330 core

flat in int fgetsLighting;

in vec3 fpos;
in vec3 n_vec;

in vec3 famb;
in vec3 fdiff;
in vec3 fspec;
in float fshin;

in vec3 fs_l_pos;

uniform vec3 obs;
uniform vec3 l_col;
uniform vec3 amb_l_col;

out vec4 FragColor;

uniform float fadeDistance = 7;

uniform int usesFog;

vec3 calc_amb() {
    return amb_l_col*famb;
    
}

vec3 calc_diff() {
    
    vec3 L = normalize(fs_l_pos - fpos);
    return (fdiff*l_col*max(dot(n_vec, L), 0.0));
    
}

// No funciona!
vec3 calc_spec() {
    vec3 v = normalize(obs - fpos);
    vec3 L = fs_l_pos - fpos;
    vec3 R = normalize(2*n_vec*dot(n_vec, L) - L);

    if (dot(normalize(n_vec), normalize(L)) > 0) {
        return l_col*fspec*max(pow(dot(R,v), fshin), 0.0);
    }
    else {
        return vec3(0.0,0.0,0.0);
    }
}



void main() {
    vec3 fgColor;
    vec3 bColor = vec3(0.105, 0.058, 0.5);

    if (fgetsLighting == 1) {
        fgColor = calc_amb() + calc_diff();
    }
    else {
        fgColor = fdiff;
    }

    if (usesFog == 1) {
        float dist = length(fpos);
        float t = clamp(dist / fadeDistance, 0.0, 1.0);
        vec3 finalColor = mix(fgColor, bColor, t);
        FragColor = vec4(finalColor, 1.0);
    }
    else {
        FragColor = vec4(fgColor, 1.0);
    }
}