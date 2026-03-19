#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;   // Matches C++ location 2
layout (location = 3) in vec2 aTexCoords; // Matches C++ location 3

out vec2 TexCoords;
out vec3 FragPos;
out mat3 TBN; // We send this to the Fragment Shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoords = aTexCoords;

    // Create the TBN Matrix
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    
    // Re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    TBN = mat3(T, B, N);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}