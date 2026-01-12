#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN; // Received from Vertex Shader

uniform sampler2D diffuseMap; // The planet surface colors
uniform sampler2D normalMap;  // The purple detail map
uniform vec3 sunPos;          // Position of your Sun light
uniform vec3 viewPos;         // Camera position (for shininess/specular)
uniform bool isSun;           // To skip lighting if we are drawing the Sun itself

void main() {
    if(isSun) {
        // The Sun just glows with its own texture
        FragColor = texture(diffuseMap, TexCoords);
    } else {
        // 1. UNPACK NORMAL: Get purple color [0,1] and convert to vector [-1,1]
        vec3 normal = texture(normalMap, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);   
        
        // 2. TRANSFORM: Rotate the normal using the TBN matrix to follow the sphere
        normal = normalize(TBN * normal);

        // 3. LIGHTING CALCULATIONS
        // Ambient (base dark light)
        vec3 ambient = 0.05 * vec3(1.0);
        
        // Diffuse (sunlight hitting the surface)
        vec3 lightDir = normalize(sunPos - FragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 0.9); // Slightly warm sun color

        // 4. FINAL COLOR
        vec3 lighting = ambient + diffuse;
        FragColor = vec4(lighting, 1.0) * texture(diffuseMap, TexCoords);
    }
}