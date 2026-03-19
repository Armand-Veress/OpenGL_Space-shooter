#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN; // Received from Vertex Shader

uniform sampler2D diffuseMap; 
uniform sampler2D normalMap;  
uniform vec3 sunPos;          
uniform vec3 viewPos;         
uniform bool isSun;           
uniform vec3 customColor;
uniform bool useColor;
uniform float alpha;
uniform bool isSaturnRing;

void main() {
    if (isSaturnRing) {
        vec2 pos = TexCoords - vec2(0.5);
        float r = length(pos);
        float angle = atan(pos.y, pos.x); // Rezultat între -PI și PI

        vec2 polarUV;
        polarUV.x = angle / (2.0 * 3.14159) + 0.5; 
        polarUV.y = (r - 0.15) / (0.5 - 0.15);     

        FragColor = texture(diffuseMap, polarUV);

        if (r > 0.48 || r < 0.15) {
            discard;
        }
    }

    if(isSun) {
        if(useColor) {
            FragColor = vec4(customColor, alpha);
        } else {
            FragColor = texture(diffuseMap, TexCoords);
        }
    } else {
        vec3 normal = texture(normalMap, TexCoords).rgb;
        normal = normalize(normal * 2.0 - 1.0);   
        normal = normalize(TBN * normal);

        vec3 ambient = 0.25 * vec3(1.0);
        
        vec3 lightDir = normalize(sunPos - FragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 0.9); 

        vec3 lighting = ambient + diffuse;
        FragColor = vec4(lighting, 1.0) * texture(diffuseMap, TexCoords);
    }
}