/***********************************
** SECTION 1: LIBRARIES & IMPORTS **
***********************************/

//* Sean T. Barrett's Image Loader
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//* OpenGL Loader Generator    (Function loader - links code to GPU)
#include <glad/glad.h>

//* Graphics Library FrameWork (Ressource manager - deals with the OS)
#include <GLFW/glfw3.h>

//* OpenGL Maths
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <vector>
#include <cstdlib> // pt rand()
#include <ctime>   // pt time()

#include "Shader.h"


/********************************
** SECTION 2: GLOBAL VARIABLES **
********************************/

//* Display & Window Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//* Camera Definition
glm::vec3 cameraPos    = glm::vec3(4500.0f, 0.0f, 0.0f);
glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp     = glm::vec3(0.0f, 1.0f, 0.0f);

//* Camera Movement
float yaw   = -90.0f; 
float pitch =  0.0f;
float constantSpeed = 100.0f; 
float cameraSpeed = 5000.0f;

//* Mouse Cursor Control
bool firstMouse = true;
float lastX =  SCR_WIDTH  / 2.0f;
float lastY =  SCR_HEIGHT / 2.0f;

//* Timing & Physics
float deltaTime = 0.0f; 
float lastFrame = 0.0f;
float globalOrbitSpeed = 0.1f;

// GAMEPLAY GLOBALS  
bool isGameOver = false;
float lastSpawnTime = 0.0f;

struct Asteroid {
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 rotationAxis;
    float scale;
    bool isActive;
};
std::vector<Asteroid> asteroids; // Lista dinamica

//* Object data

// Universe Constants
glm::vec3 universeOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

glm::vec3 universePos = universeOrigin; 
float universeScale = 5000.0f; 

glm::vec3 sunPos = universeOrigin; 
float sunScale = 1000.0f;

glm::vec3 mercuryPos = glm::vec3(3000.0f, 0.0f, 0.0f);
float mercuryScale = 38.0f; 
float mercuryOrbitRadius = 3000.0f; 
float mercuryOrbitSpeed = 0.83f*globalOrbitSpeed;

glm::vec3 venusPos = glm::vec3(4000.0f, 0.0f, 0.0f);
float venusScale = 95.0f; 
float venusOrbitRadius = 4000.0f; 
float venusOrbitSpeed = 0.32f*globalOrbitSpeed;

glm::vec3 earthPos = glm::vec3(5000.0f, 0.0f, 0.0f);
float earthScale = 100.0f; 
float earthOrbitRadius = 5000.0f; 
float earthOrbitSpeed = 0.1f*globalOrbitSpeed;

glm::vec3 moonPos = glm::vec3(5250.0f, 0.0f, 0.0f);
float moonScale = 20.0f; 
float moonOrbitRadius = 370.0f; 
float moonOrbitSpeed = 0.8f*globalOrbitSpeed;

glm::vec3 marsPos = glm::vec3(6000.0f, 0.0f, 0.0f);
float marsScale = 53.0f; 
float marsOrbitRadius = 6000.0f; 
float marsOrbitSpeed = 0.32f*globalOrbitSpeed;

glm::vec3 jupiterPos = glm::vec3(9000.0f, 0.0f, 0.0f);
float jupiterScale = 700.0f; 
float jupiterOrbitRadius = 9000.0f; 
float jupiterOrbitSpeed = 0.08f*globalOrbitSpeed;

glm::vec3 saturnPos = glm::vec3(12000.0f, 0.0f, 0.0f);
float saturnScale = 550.0f; 
float saturnOrbitRadius = 12000.0f; 
float saturnOrbitSpeed = 0.03f*globalOrbitSpeed;

glm::vec3 uranusPos = glm::vec3(14000.0f, 0.0f, 0.0f);
float uranusScale = 350.0f; 
float uranusOrbitRadius = 14000.0f; 
float uranusOrbitSpeed = 0.01f*globalOrbitSpeed;

glm::vec3 neptunePos = glm::vec3(16000.0f, 0.0f, 0.0f);
float neptuneScale = 300.0f; 
float neptuneOrbitRadius = 16000.0f; 
float neptuneOrbitSpeed = 0.006f*globalOrbitSpeed;

//* GPU Resource Handles - Render IDs
    
    // --- VAO & VBO ---
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    unsigned int sphereIndexCount;
    unsigned int ringVAO, ringVBO;
    unsigned int shipVAO, shipVBO;
    unsigned int crosshairVAO, crosshairVBO; // (NOU) Tinta
    
    // --- Universe Stars-Background ---
    unsigned int universeTexture;
    
    // --- Sun ---
    unsigned int sunTexture;

    // --- Mercury ---
    unsigned int mercuryTexture;
    unsigned int mercuryNormalMap;

    // --- Venus ---
    unsigned int venusTexture;
    unsigned int venusNormalMap;

    // --- Earth ---
    unsigned int earthTexture;
    unsigned int earthNormalMap;

    // --- Moon ---
    unsigned int moonTexture;
    unsigned int moonNormalMap;

    // --- Mars ---
    unsigned int marsTexture;
    unsigned int marsNormalMap;

    // --- Jupiter ---
    unsigned int jupiterTexture;

    // --- Saturn ---
    unsigned int saturnTexture;
    unsigned int saturnRingTexture;

    // --- Uranus ---
    unsigned int uranusTexture;

    // --- Neptun ---
    unsigned int neptuneTexture;
 
    // --- Defauld Flat Normal Map ---
    unsigned int flatNormalMap;

    // --- ASTEROID TEXTURE  ---
    unsigned int asteroidTexture;

    unsigned int asteroidVAO, asteroidVBO;
      

/************************************************
** SECTION 3: INTERACTIVITY & EVENT MANAGEMENT **
************************************************/

//* Window Resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// RESET GAME LOGIC 
void resetGame() {
    asteroids.clear();
    isGameOver = false;
    cameraPos = glm::vec3(4500.0f, 0.0f, 0.0f);
    std::cout << "GAME RESTARTED!" << std::endl;
}

// SHOOTING LOGIC  
void shoot() {
    if (isGameOver) return;
    
    for (auto &ast : asteroids) {
        if (!ast.isActive) continue;

        glm::vec3 toAst = ast.position - cameraPos;
        float t = glm::dot(toAst, cameraLookAt);

        if (t < 0.0f) continue; 

        glm::vec3 pointOnRay = cameraPos + cameraLookAt * t;
        float dist = glm::distance(pointOnRay, ast.position);

        if (dist < ast.scale * 1.5f) {
            ast.isActive = false; 
            std::cout << "Asteroid Destroyed!" << std::endl;
            break; 
        }
    }
}

// SPAWN LOGIC
void spawnAsteroid() {
    Asteroid a;
    //Random position
    float angle = (float)(rand() % 360);
    float dist = 7000.0f + (rand() % 2000);
    float height = (float)((rand() % 800) - 400);
    
    a.position = glm::vec3(std::sin(angle) * dist, height, std::cos(angle) * dist);
    
    // Speed towrds earth
    glm::vec3 dir = glm::normalize(earthPos - a.position);
    float speed = 400.0f + (rand() % 400);
    a.velocity = dir * speed;
    
    a.rotationAxis = glm::normalize(glm::vec3(rand(), rand(), rand()));
    a.scale = 25.0f + (rand() % 50);
    a.isActive = true;
    
    asteroids.push_back(a);
}

//  INPUT CALLBACKS 
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        shoot();
    }
}

//* Camera Keyboard Control 
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    // RESTART
    if (isGameOver) {
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            resetGame();
        }
        return; // Oprim miscarea daca e game over
    }

    float speed = cameraSpeed * deltaTime; 

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += speed * cameraLookAt;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= speed * cameraLookAt;

    glm::vec3 rightVector = glm::normalize(glm::cross(cameraLookAt, cameraUp));
     
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += rightVector * speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= rightVector * speed;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos += cameraUp * speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraPos -= cameraUp * speed;
}

//* Camera Cursor Control
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    if (isGameOver) return; // Blocare mouse la game over

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; 
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Constrain pitch so the screen doesn't flip
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update cameraLookAt vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraLookAt = glm::normalize(front);
}


/*********************************
** SECTION 4: UTILITY FUNCTIONS **
*********************************/

//* Loading External Texture Assets
unsigned int loadTexture(char const * path, bool isAlpha) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    
    // Flip only for rings/transparent textures if needed
    stbi_set_flip_vertically_on_load(isAlpha); 

    // Force 4 channels if isAlpha is true, otherwise let stb detect
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, isAlpha ? 4 : 0);
     
    if (data) {
        GLenum format;
        if (isAlpha) {
            format = GL_RGBA;
        } else {
            if (nrComponents == 1) format = GL_RED;
            else if (nrComponents == 3) format = GL_RGB;
            else if (nrComponents == 4) format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // RING SETTINGS: Clamp to edge prevents seams
        // PLANET SETTINGS: Repeat for tiling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, isAlpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, isAlpha ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "SUCCESS: Loaded " << (isAlpha ? "ALPHA " : "") << "texture: " << path << std::endl;
    } else {
        std::cout << "ERROR: Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//* Calculate Rotation Orbit
glm::vec3 calculateOrbit(glm::vec3 center, float radius, float speed, float time) {
    float angle = time * speed;
    float x = center.x + std::cos(angle) * radius;
    float z = center.z + std::sin(angle) * radius;
    float y = center.y; // Menținem orbita pe un plan plat (XZ)

    return glm::vec3(x, y, z);
}


/******************************
** SECTION 5: GEOMETRY SETUP **
******************************/
// !-- run once (raw data -> GPU memory)

//* Builds a unit sphere centered around the origin
void setupSphere() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359f;

    for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = std::cos(ySegment * PI);
            float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float uTex = xSegment;
            float vTex = ySegment;

            // Normal
            glm::vec3 normal(xPos, yPos, zPos);

            // Tangent
            float tx = -std::sin(xSegment * 2.0f * PI);
            float ty = 0.0f;
            float tz = std::cos(xSegment * 2.0f * PI);
            glm::vec3 tangent(tx, ty, tz);

            vertices.push_back(xPos); vertices.push_back(yPos); vertices.push_back(zPos);
            vertices.push_back(normal.x); vertices.push_back(normal.y); vertices.push_back(normal.z);
            vertices.push_back(tangent.x); vertices.push_back(tangent.y); vertices.push_back(tangent.z);
            vertices.push_back(uTex); vertices.push_back(vTex);
        }
    }

    for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
        for (unsigned int x = 0; x < X_SEGMENTS; ++x) {
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            indices.push_back(y * (X_SEGMENTS + 1) + x);
            indices.push_back(y * (X_SEGMENTS + 1) + (x + 1));

            indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            indices.push_back(y * (X_SEGMENTS + 1) + (x + 1));
            indices.push_back((y + 1) * (X_SEGMENTS + 1) + (x + 1));
        }
    }
    sphereIndexCount = static_cast<unsigned int>(indices.size());

    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereEBO);

    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    int stride = 11 * sizeof(float);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);  
}

//* Spaceship's nose
void setupNose() {
    float noseVertices[] = {
         0.0f,  0.05f, 0.0f, 
        -0.1f, -0.10f, 0.0f, 
         0.1f, -0.10f, 0.0f  
    };
    glGenVertexArrays(1, &shipVAO);
    glGenBuffers(1, &shipVBO);
    glBindVertexArray(shipVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(noseVertices), noseVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

// Setup CROSSHAIR
void setupCrosshair() {
    float chVertices[] = {
        -0.03f, 0.0f, 0.0f,   0.03f, 0.0f, 0.0f, // Orizontal
         0.0f, -0.04f, 0.0f,  0.0f, 0.04f, 0.0f  // Vertical
    };
    glGenVertexArrays(1, &crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindVertexArray(crosshairVAO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chVertices), chVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

//* Cubes
void setupAsteroids() {
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f
    };
    glGenVertexArrays(1, &asteroidVAO);
    glGenBuffers(1, &asteroidVBO);
    glBindVertexArray(asteroidVAO);
    glBindBuffer(GL_ARRAY_BUFFER, asteroidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

//* Saturn's Ring
void setupRing() {
    float ringVertices[] = {
        // Pos                // Norm             // Tangent          // UV
        -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

        -1.0f, 0.0f,  1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        -1.0f, 0.0f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f
    };

    glGenVertexArrays(1, &ringVAO);
    glGenBuffers(1, &ringVBO);

    glBindVertexArray(ringVAO);
    glBindBuffer(GL_ARRAY_BUFFER, ringVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ringVertices), ringVertices, GL_STATIC_DRAW);

    int stride = 11 * sizeof(float);

    // POSITION
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    // NORMAL
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TANGENT
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // UV
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(9 * sizeof(float)));
    glEnableVertexAttribArray(3);
}

/*******************************
** SECTION 6: RENDERING LOGIC **
*******************************/
// !-- run every frame

//* Universe Stars-Background
void drawUniverse(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", true);

    glm::mat4 viewStatic = glm::mat4(glm::mat3(view)); 
    shader.setMat4("view", viewStatic);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(universeScale)); 
    shader.setMat4("model", model);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, universeTexture);
    shader.setInt("diffuseMap", 0);

    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
    glEnable(GL_CULL_FACE);
}

//* Sun
void drawSun(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE); 
    shader.use();
    shader.setBool("isSun", true); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sunTexture); 
    shader.setInt("diffuseMap", 0); // Link the texture to the shader sampler

    // View & Projection Matrices
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, sunPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(sunScale));
    shader.setMat4("model", model);

    //  Draw
    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Mercury
void drawMercury(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mercuryTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mercuryNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, mercuryPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(mercuryScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Venus 
void drawVenus(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, venusTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, venusNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, venusPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(venusScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Planet Earth
void drawEarth(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, earthTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, earthNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, earthPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(earthScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Earth's Moon
void drawMoon(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, moonTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, moonNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, moonPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(moonScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Mars
void drawMars(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, marsTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, marsNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, marsPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(marsScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Jupiter
void drawJupiter(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, jupiterTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, jupiterPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(jupiterScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}

//* Saturn
void drawSaturn(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, saturnTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, saturnPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(saturnScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}


//* Saturn's Ring

void drawRing(Shader &shader, glm::mat4 projection, glm::mat4 view, 
              glm::vec3 planetPos, float planetScale, unsigned int textureID) {
    
    // STATE: BLENDING
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // STATE: DEPTH & CULLING
    glDepthMask(GL_FALSE);
    glDisable(GL_CULL_FACE);

    shader.use();
    shader.setBool("isSun", true);
    shader.setBool("isRing", true); 

    // UNIFORMS: MATRICES
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, planetPos); 
    float ringScale = planetScale * 2.2f; 
    model = glm::scale(model, glm::vec3(ringScale, 1.0f, ringScale));
    shader.setMat4("model", model);

    // TEXTURE: DIFFUSE
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    shader.setInt("diffuseMap", 0);

    // TEXTURE: NORMAL (FLAT)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    // RENDER
    glBindVertexArray(ringVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // RESTORE STATE
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE); 
}


//* Uranus
void drawUranus(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, uranusTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, uranusPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(uranusScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}


//* Neptun
void drawNeptune(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", false);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, neptuneTexture);
    shader.setInt("diffuseMap", 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, neptunePos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(neptuneScale));
    shader.setMat4("model", model);

    glBindVertexArray(sphereVAO); 
    glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    glEnable(GL_CULL_FACE);
}


//* Spaceship's nose
void drawSpaceshipNose(Shader &shader, glm::mat4 projection) {
    shader.use();
    shader.setMat4("projection", projection);
    
    glm::mat4 view = glm::mat4(1.0f); 
    shader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
 
    model = glm::translate(model, glm::vec3(0.0f, -0.6f, -1.5f)); 
    model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    shader.setMat4("model", model);

    glBindVertexArray(shipVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// Dynamic Asteroids
void drawDynamicAsteroids(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    shader.use();
    shader.setBool("isSun", false); // Sa aiba umbre
    shader.setBool("isRing", false);

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, asteroidTexture);
    shader.setInt("diffuseMap", 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    glBindVertexArray(sphereVAO); // Folosim sferul pt aspect natural

    for (const auto& ast : asteroids) {
        if (!ast.isActive) continue;

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, ast.position);
        
        float angle = (float)glfwGetTime() * 50.0f;
        model = glm::rotate(model, glm::radians(angle), ast.rotationAxis);
        model = glm::scale(model, glm::vec3(ast.scale));

        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    }
}

// Draw Crosshair
void drawCrosshair(Shader &shader) {
    glDisable(GL_DEPTH_TEST); 
    shader.use();
    shader.setBool("isSun", true); 
    shader.setBool("isRing", false);

    shader.setMat4("projection", glm::mat4(1.0f));
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("model", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, flatNormalMap); shader.setInt("diffuseMap", 0);

    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST);
}


int main(){


/*************************************
** SECTION 7: SYSTEM INITIALIZATION **
*************************************/

//* Initialize GLFW & Create Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGRMR - OpenGL project", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

//* Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

//* Set Viewport & Callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Screenmapping (NDC -> 2D Screen Space)
    glViewport(0, 0, width, height);

//* Input Configurations

    // Hide Cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set Mouse Callback
    glfwSetCursorPosCallback(window, mouse_callback);

    // Callback Click Mouse
    glfwSetMouseButtonCallback(window, mouse_button_callback);


/*********************************************
** SECTION 8: GLOBAL OPENGL CONFIGURATIONS  **
*********************************************/

    // Depth Testing
    glEnable(GL_DEPTH_TEST);

    // Culling


/*********************************************
** SECTION 9: GLOBAL OPENGL CONFIGURATIONS  **
*********************************************/

    // Geometric Shapes Models
    setupSphere();
    setupRing();

    // Spaceship's nose model
    setupNose();

    // Cubes model
    setupAsteroids(); // (Folosit doar pt incarcarea VBO vechi, nu pt logica noua)
    
    // (NOU) Setup Crosshair
    setupCrosshair();


/****************************************************
** SECTION 10: ASSETS LOADING (SHADERS & TEXTURES) **
****************************************************/

//* Load Shaders

    Shader mainShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    mainShader.use();
    glUniform1i(glGetUniformLocation(mainShader.ID, "texture_diffuse1"), 0);

//* Load Textures
    // Universe Stars-Background texture
    universeTexture = loadTexture("textures/8k_stars_milky_way.jpg", false);

    // Sun's texture
    sunTexture = loadTexture("textures/8k_sun.jpg", false);

    // Mercury's texture
    mercuryTexture = loadTexture("textures/8k_mercury.jpg", false);
    mercuryNormalMap = loadTexture("textures/mercury_normal.png", false);

    // Venus's texture
    venusTexture = loadTexture("textures/8k_venus_surface.jpg", false);
    venusNormalMap = loadTexture("textures/venus_normal.png", false);

    // Earth's texture
    earthTexture = loadTexture("textures/8k_earth_daymap.jpg", false);
    earthNormalMap = loadTexture("textures/earth_normal.png", false);

    // Moon's texture
    moonTexture = loadTexture("textures/8k_moon.jpg", false);
    moonNormalMap = loadTexture("textures/moon_normal.png", false);
    
    // Mars's texture
    marsTexture = loadTexture("textures/8k_mars.jpg", false);
    marsNormalMap = loadTexture("textures/mars_normal.png", false);

    // Jupiter's texture
    jupiterTexture = loadTexture("textures/8k_jupiter.jpg", false);

    // Saturns's texture
    saturnTexture = loadTexture("textures/8k_saturn.jpg", false);
    saturnRingTexture = loadTexture("textures/8k_saturn_ring.png", true);

    // Uranus's texture
    uranusTexture = loadTexture("textures/2k_uranus.jpg", false);

    // Neptune's texture
    neptuneTexture = loadTexture("textures/2k_neptune.jpg", false);
    
    // Asteroid texture
    asteroidTexture = loadTexture("textures/4k_makemake.jpg", false);

    // Generate Default Flat Normal Map
    glGenTextures(1, &flatNormalMap);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    unsigned char pixel[] = { 128, 128, 255, 255 }; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Init Random seed
    std::srand(static_cast<unsigned int>(std::time(0)));

    while (!glfwWindowShouldClose(window)) {


/************************************
** SECTION 11: FRAME LOGIC & CLEAR **
************************************/

    //* Delta Time      
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        mercuryPos = calculateOrbit(sunPos, mercuryOrbitRadius, mercuryOrbitSpeed, currentFrame);
        venusPos = calculateOrbit(sunPos, venusOrbitRadius, venusOrbitSpeed, currentFrame);
        earthPos = calculateOrbit(sunPos, earthOrbitRadius, earthOrbitSpeed, currentFrame);
        moonPos = calculateOrbit(earthPos, moonOrbitRadius, moonOrbitSpeed, currentFrame);
        marsPos = calculateOrbit(sunPos, marsOrbitRadius, marsOrbitSpeed, currentFrame);
        jupiterPos = calculateOrbit(sunPos, jupiterOrbitRadius, jupiterOrbitSpeed, currentFrame);
        saturnPos = calculateOrbit(sunPos, saturnOrbitRadius, saturnOrbitSpeed, currentFrame);
        uranusPos = calculateOrbit(sunPos, uranusOrbitRadius, uranusOrbitSpeed, currentFrame);
        neptunePos = calculateOrbit(sunPos, neptuneOrbitRadius, neptuneOrbitSpeed, currentFrame);
 
    //* Process Input
        processInput(window);

    //* Clear Buffers
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // --- LOGICA JOC (NOU) ---
    if (!isGameOver) {
        // Spawn asteroizi (la fiecare 2 secunde)
        if (currentFrame - lastSpawnTime > 2.0f) {
            spawnAsteroid();
            lastSpawnTime = currentFrame;
        }

        for (auto &ast : asteroids) {
            if (!ast.isActive) continue;

            // Miscarea asteroidului
            ast.position += ast.velocity * deltaTime;

            // Coliziune Pamant
            float distToEarth = glm::distance(earthPos, ast.position);
            // Pamantul are scale 100 (radius ~50), asteroid ~25 + margine
            if (distToEarth < (50.0f + ast.scale)) {
                std::cout << "GAME OVER! Press R to Restart." << std::endl;
                isGameOver = true;
            }

            // Cleanup (daca se duce prea departe)
            if (glm::distance(sunPos, ast.position) > 20000.0f) {
                ast.isActive = false;
            }
        }
    }


/***********************************************
** SECTION 12: CAMERA AND PROJECTION MATRICES **
***********************************************/

    //* Window's Aspect Ratio
        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        float aspect = (float)currentWidth / (float)currentHeight;

    //* Projection Matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 10000.0f);
        
    //* View Matrix
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraLookAt, cameraUp);


/********************************
** SECTION 13: SCENE RENDERING **
********************************/
    float farLimit = 100000.0f; 
    glm::mat4 farProjection = glm::perspective(glm::radians(45.0f), aspect, 10.0f, farLimit);

    mainShader.use();
    mainShader.setBool("isRing", false); 
    mainShader.setVec3("sunPos", sunPos);
    mainShader.setVec3("viewPos", cameraPos);

    drawUniverse(mainShader, farProjection, view);
    drawMercury(mainShader, farProjection, view);
    drawVenus(mainShader, farProjection, view);
    drawSun(mainShader, farProjection, view);
    drawEarth(mainShader, farProjection, view);
    drawMoon(mainShader, farProjection, view);
    drawMars(mainShader, farProjection, view);
    drawJupiter(mainShader, farProjection, view);
    drawSaturn(mainShader, farProjection, view);
    drawRing(mainShader, farProjection, view, saturnPos, saturnScale, saturnRingTexture);
    drawUranus(mainShader, farProjection, view);
    drawNeptune(mainShader, farProjection, view);
    
// --- THE TRICK: CLEAR DEPTH ---
// This prevents the background from "cutting" anything we draw next.
// It keeps the pixels on screen but resets the Z-buffer.
    glClear(GL_DEPTH_BUFFER_BIT); 

    glm::mat4 localProjection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 5000.0f);

    // (NOU) Deseneaza asteroizii
    drawDynamicAsteroids(mainShader, projection, view);

    // (NOU) Tinta (Crucea)
    drawCrosshair(mainShader);

/**********************************************
** SECTION 14: BUFFER SWAP AND EVENT POLLING **
**********************************************/
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

/************************
** SECTION 0: CLEAN UP **
************************/
    glfwTerminate();
    return 0;
}