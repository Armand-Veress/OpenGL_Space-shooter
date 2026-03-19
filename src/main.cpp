
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

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Shader.h"

//* IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "UI.h"


/********************************
** SECTION 2: GLOBAL VARIABLES **
********************************/

// -->  CAMERA DEFINITION
    glm::vec3 cameraPos   = glm::vec3(45000.0f, 500.0f, 0.0f);
    glm::vec3 cameraLookAt = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

// -->  CAMERA MOVEMENT
    float yaw   = 180.0f; 
    float pitch =  0.0f;
    float constantSpeed = 500.0f; 

// -->  CURSOR CONTROL
    bool firstMouse = true;
    float lastX;
    float lastY;

// -->  TIMING & PHYSICS
    float deltaTime = 0.0f; 
    float lastFrame = 0.0f;
    float globalOrbitSpeed = 0.1f;

    // End of Earth flash
    float flashAlpha = 0.0f;
    float flashSpeed = 0.8f;

    // Spaceship Behaviour & Controls
    float lerpFactor = 2.5f; 
    float mouseSensitivity = 0.15f; 
    float keyboardSensitivity = 60.0f;
    float mouseXVel = 0.0f;
    float mouseYVel = 0.0f;
    float targetYawSpeed = 0.0f;
    float targetPitchSpeed = 0.0f;
    float currentYawSpeed = 0.0f;
    float currentPitchSpeed = 0.0f;
    float shipRoll = 0.0f;  
    float shipPitch = 0.0f; 
    float mouseXOffset = 0.0f;
    float mouseYOffset = 0.0f;

    // Lasers Behaviour
    float laserSpeed = 5000.0f;
    float laserSize = 0.25f;
    float lastShotTime = 0.0f;

    // Asteroids Behaviour
    float spawnDistance = 8000.0f; 
    float minAsteroidSpeed = 30.0f;
    float maxAsteroidSpeed = 100.0f;
    float minAsteroidSize = 10.0f;
    float maxAsteroidSize = 75.0f;

    // Ship-Planet Collision
    struct PlanetCollision {
        glm::vec3 position;
        float radius;
    };

// -->  GAMEPLAY GLOBALS  
    bool isGameOver = false;
    float gameStartTime = 0.0f;
    float survivalTime = 0.0f;
    int destroyedAsteroids = 0;
    float gameScore = 0.0f;
    float lastSpawnTime = 0.0f;
    bool earthIsHit = false;


// -->  OBJECT DATA

    //* Universe
    glm::vec3 universeOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

    //* Stars Background
    glm::vec3 universePos = universeOrigin; 
    float universeScale = 5000.0f; 

    //* Sun
    glm::vec3 sunPos = universeOrigin; 
    float sunScale = 1000.0f;

    //* Mercury
    glm::vec3 mercuryPos = glm::vec3(3000.0f, 0.0f, 0.0f);
    float mercuryScale = 38.0f; 
    float mercuryOrbitRadius = 3000.0f; 
    float mercuryOrbitSpeed = 0.83f*globalOrbitSpeed;

    //* Venus
    glm::vec3 venusPos = glm::vec3(4000.0f, 0.0f, 0.0f);
    float venusScale = 95.0f; 
    float venusOrbitRadius = 4000.0f; 
    float venusOrbitSpeed = 0.32f*globalOrbitSpeed;

    //* Earth
    glm::vec3 earthPos = glm::vec3(5000.0f, 0.0f, 0.0f);
    float earthScale = 100.0f; 
    float earthOrbitRadius = 5000.0f; 
    float earthOrbitSpeed = 0.1f*globalOrbitSpeed;

    //* Moon
    glm::vec3 moonPos = glm::vec3(5250.0f, 0.0f, 0.0f);
    float moonScale = 20.0f; 
    float moonOrbitRadius = 370.0f; 
    float moonOrbitSpeed = 0.8f*globalOrbitSpeed;

    //* Mars
    glm::vec3 marsPos = glm::vec3(6000.0f, 0.0f, 0.0f);
    float marsScale = 53.0f; 
    float marsOrbitRadius = 6000.0f; 
    float marsOrbitSpeed = 0.32f*globalOrbitSpeed;

    //* Jupiter
    glm::vec3 jupiterPos = glm::vec3(9000.0f, 0.0f, 0.0f);
    float jupiterScale = 700.0f; 
    float jupiterOrbitRadius = 9000.0f; 
    float jupiterOrbitSpeed = 0.08f*globalOrbitSpeed;

    //* Saturn
    glm::vec3 saturnPos = glm::vec3(12000.0f, 0.0f, 0.0f);
    float saturnScale = 550.0f; 
    float saturnOrbitRadius = 12000.0f; 
    float saturnOrbitSpeed = 0.03f*globalOrbitSpeed;

    //* Uranus
    glm::vec3 uranusPos = glm::vec3(14000.0f, 0.0f, 0.0f);
    float uranusScale = 350.0f; 
    float uranusOrbitRadius = 14000.0f; 
    float uranusOrbitSpeed = 0.01f*globalOrbitSpeed;

    //* Neptune
    glm::vec3 neptunePos = glm::vec3(16000.0f, 0.0f, 0.0f);
    float neptuneScale = 300.0f; 
    float neptuneOrbitRadius = 16000.0f; 
    float neptuneOrbitSpeed = 0.006f*globalOrbitSpeed;

    //* Spaceship 
    float shipScale = 2.0f;
    float shipLength = 5.0f;
    float shipWidth = 0.5f;
    float shipHeight = 0.1;

    //* Asteroids
    struct Asteroid {
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 rotationAxis;
        float rotationSpeed;
        float scale;
        bool isActive;
    };
    std::vector<Asteroid> asteroids; 

    //* Explosions
    struct Explosion {
        glm::vec3 position;
        float size;
        float alpha;
        float lifeTime;
        float maxSize;
    };
    std::vector<Explosion> activeExplosions;

    //* Lasers
    struct LaserBeam {
        glm::vec3 position;
        glm::vec3 direction;
        float speed;
        float lifeTime;
        std::vector<glm::vec3> trail;
    };
    std::vector<LaserBeam> activeLasers;

// -->  GPU Resource Handles - Render IDs
    
    // --- VAO & VBO ---
    unsigned int sphereVAO, sphereVBO, sphereEBO;
    unsigned int sphereIndexCount;
    unsigned int ringVAO, ringVBO;
    unsigned int shipVAO, shipVBO, shipEBO;
    unsigned int crosshairVAO, crosshairVBO; 
    unsigned int asteroidVAO, asteroidVBO;
    
    // --- TEXTURES ---
    unsigned int universeTexture;
    unsigned int sunTexture;
    unsigned int mercuryTexture;
    unsigned int venusTexture;
    unsigned int earthTexture;
    unsigned int moonTexture;
    unsigned int marsTexture;
    unsigned int jupiterTexture;
    unsigned int saturnTexture;
    unsigned int saturnRingTexture;
    unsigned int uranusTexture;
    unsigned int neptuneTexture;
    unsigned int shipTexture;
    unsigned int asteroidTexture;
 
    // --- NORMAL MAPS ---
    unsigned int mercuryNormalMap;
    unsigned int venusNormalMap;
    unsigned int earthNormalMap;
    unsigned int moonNormalMap;
    unsigned int marsNormalMap;
    unsigned int flatNormalMap; // default uniform Normal Map


/************************************************
** SECTION 3: INTERACTIVITY & EVENT MANAGEMENT **
************************************************/

//* RESET GAME
void resetGame() {
    asteroids.clear();
    activeLasers.clear();
    destroyedAsteroids = 0;
    survivalTime = 0.0f;
    gameStartTime = (float)glfwGetTime(); // Resetăm ceasul
    isGameOver = false;
    cameraPos = glm::vec3(4500.0f, 0.0f, 0.0f);
}

//* Keyboard Steering & Shooting
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // [MOVEMENT]
    float currentSpeed = constantSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || 
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        currentSpeed *= 4.0f;
    }
    cameraPos += cameraLookAt * currentSpeed * deltaTime;

    // [ROTATION TARGETS]
    targetPitchSpeed = 0.0f;
    targetYawSpeed = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) targetPitchSpeed = keyboardSensitivity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) targetPitchSpeed = -keyboardSensitivity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) targetYawSpeed = -keyboardSensitivity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) targetYawSpeed = keyboardSensitivity;

    // Add Mouse velocity to targets (accumulated from callback)
    targetYawSpeed += mouseXVel;
    targetPitchSpeed += mouseYVel;

    // [LERP / SMOOTHING]
    currentPitchSpeed = glm::mix(currentPitchSpeed, targetPitchSpeed, lerpFactor * deltaTime);
    currentYawSpeed = glm::mix(currentYawSpeed, targetYawSpeed, lerpFactor * deltaTime);

    yaw   += currentYawSpeed * deltaTime;
    pitch += currentPitchSpeed * deltaTime;

    // Accumulation reset
    mouseXVel = 0.0f;
    mouseYVel = 0.0f;

    // [CONSTRAINTS & VECTORS]
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraLookAt = glm::normalize(front);

    // [VISUAL LAG]
    mouseXOffset = currentYawSpeed * 0.05f;
    mouseYOffset = currentPitchSpeed * 0.05f;

    // [LASERS]
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || 
        glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        float currentTime = (float)glfwGetTime();
        if (currentTime - lastShotTime > 0.15f) {
            LaserBeam newLaser;
            newLaser.position = cameraPos + (cameraLookAt * shipLength) + (cameraUp * -0.25f);
            newLaser.direction = cameraLookAt;
            newLaser.speed = laserSpeed; 
            newLaser.lifeTime = 5.0f;
            activeLasers.push_back(newLaser);
            lastShotTime = currentTime;
        }
    }
}


//* Cursor Steering
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos; lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos; lastY = ypos;

    mouseXVel = xoffset * mouseSensitivity * 100.0f; 
    mouseYVel = -yoffset * mouseSensitivity * 100.0f;
}


//* SPAWN LOGIC
void spawnAsteroid() {
    Asteroid a;
    float theta = ((float)rand() / RAND_MAX) * 2.0f * 3.14159f; 
    float phi = acos(2.0f * ((float)rand() / RAND_MAX) - 1.0f); 

    a.position.x = spawnDistance * sin(phi) * cos(theta);
    a.position.y = spawnDistance * sin(phi) * sin(theta);
    a.position.z = spawnDistance * cos(phi);

    glm::vec3 dirToEarth = glm::normalize(earthPos - a.position);
    a.scale = minAsteroidSize + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (maxAsteroidSize - minAsteroidSize)));
    float speed = minAsteroidSpeed + static_cast<float>(rand() % (int)(maxAsteroidSpeed - minAsteroidSpeed));
    
    a.velocity = dirToEarth * speed;
    a.isActive = true;
    a.rotationAxis = glm::normalize(glm::vec3((rand()%100)/100.0f, (rand()%100)/100.0f, (rand()%100)/100.0f));
    a.rotationSpeed = 0.5f + static_cast<float>(rand() % 100) / 50.0f;

    asteroids.push_back(a);
}


/*********************************
** SECTION 4: UTILITY FUNCTIONS **
*********************************/

//* Loading External Texture Assets
unsigned int loadTexture(char const * path, bool isAlpha) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
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
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "SUCCESS: Loaded  texture: " << path << std::endl;
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

            glm::vec3 normal(xPos, yPos, zPos);
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

//* Spaceship
void setupShip() {
    float halfW = shipWidth / 2.0f;
    float halfH = shipHeight / 2.0f;

    float shipVertices[] = {
        // X          Y               Z               U     V
        // --- Top right side ---
         0.0f,       0.0f,           0.0f,           0.0f, 0.0f, 
         halfW,     -halfH,          0.0f,           1.0f, 0.0f, 
         0.0f,      -halfH,         -shipLength,     0.5f, 5.0f, 

        // --- Top left side ---
         0.0f,       0.0f,           0.0f,           1.0f, 0.0f, 
        -halfW,     -halfH,          0.0f,           0.0f, 0.0f, 
         0.0f,      -halfH,         -shipLength,     0.5f, 5.0f, 

        // --- Bottom right side ---
         0.0f,      -shipHeight,     0.0f,           0.0f, 0.0f, 
         halfW,     -halfH,          0.0f,           1.0f, 0.0f, 
         0.0f,      -halfH,         -shipLength,     0.5f, 5.0f, 

        // --- Bottom left side ---
         0.0f,      -shipHeight,     0.0f,           1.0f, 0.0f, 
        -halfW,     -halfH,          0.0f,           0.0f, 0.0f, 
         0.0f,      -halfH,         -shipLength,     0.5f, 5.0f
    };

    glGenVertexArrays(1, &shipVAO);
    glGenBuffers(1, &shipVBO);
    glBindVertexArray(shipVAO);
    glBindBuffer(GL_ARRAY_BUFFER, shipVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(shipVertices), shipVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(3);
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

//* Setup CROSSHAIR
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
    shader.setInt("diffuseMap", 0); 

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, sunPos); 
    model = glm::rotate(model, (float)glfwGetTime() * 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(sunScale));
    shader.setMat4("model", model);

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

//* Earth
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
    shader.setBool("isSaturnRing", true);

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, planetPos); 
    float ringScale = planetScale * 5.0f; 
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
    shader.setBool("isSaturnRing", false);

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


//* Spaceship
void drawShip(Shader &shader, glm::mat4 projection) {
    glDisable(GL_CULL_FACE);
    shader.use();
    shader.setBool("isSun", true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shipTexture);
    shader.setInt("diffuseMap", 0);

    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.5f)); 
    model = glm::rotate(model, glm::radians(shipRoll), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(shipPitch), glm::vec3(1.0f, 0.0f, 0.0f)); // ADD THIS
    model = glm::rotate(model, glm::radians(-8.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // PERSPECTIVE FIX
    model = glm::scale(model, glm::vec3(shipScale));

    shader.setMat4("model", model);

    glBindVertexArray(shipVAO);
    glDrawArrays(GL_TRIANGLES, 0, 12);
    glEnable(GL_CULL_FACE);
}

// Ship's Laser
void drawLaser(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    shader.use();
    shader.setBool("isSun", true);
    shader.setBool("useColor", true);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    for (const auto& laser : activeLasers) {
        if (glm::distance(laser.position, cameraPos) < 15.0f) continue;

        // [LASER CORE SPHERE]
        shader.setVec3("customColor", glm::vec3(1.0f, 0.5f, 1.0f)); 
        shader.setFloat("alpha", 1.0f); 
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, laser.position);
        float pulse = 2.5f + 0.2f * std::sin((float)glfwGetTime() * 15.0f);
        model = glm::scale(model, glm::vec3(pulse));
        shader.setMat4("model", model);
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

        // [OUTER GLOW SPHERE]
        shader.setVec3("customColor", glm::vec3(0.5f, 0.0f, 0.5f)); 
        shader.setFloat("alpha", 0.3f);
        
        glm::mat4 glowModel = glm::mat4(1.0f);
        glowModel = glm::translate(glowModel, laser.position);
        glowModel = glm::scale(glowModel, glm::vec3(pulse * 1.8f));
        shader.setMat4("model", glowModel);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

        // [FADED LASER TRAIL] 
        for (size_t j = 0; j < laser.trail.size(); j++) {
            float trailAlpha = (float)j / laser.trail.size() * 0.4f; 
            shader.setFloat("alpha", trailAlpha);
            
            glm::mat4 trailModel = glm::mat4(1.0f);
            trailModel = glm::translate(trailModel, laser.trail[j]);
            float trailScale = ((float)j / laser.trail.size()) * 1.8f; 
            trailModel = glm::scale(trailModel, glm::vec3(trailScale));
            
            shader.setMat4("model", trailModel);
            glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
        }
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    shader.setBool("isSun", false);
    shader.setFloat("alpha", 1.0f);
    shader.setVec3("customColor", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setBool("useColor", false);
}

//* Explosions
void drawExplosions(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    if (activeExplosions.empty()) return;

    shader.use();
    shader.setBool("isSun", true);
    shader.setBool("useColor", true);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); 
    glDepthMask(GL_FALSE);

    for (const auto& exp : activeExplosions) {
        float growthFactor = exp.size / exp.maxSize;

        
        // [OUTER EXPLOSION SPHERE]
        float auraStart = exp.maxSize * 0.25f;
        float auraCurrent = glm::mix(auraStart, exp.maxSize, growthFactor);

        glm::mat4 modelAura = glm::mat4(1.0f);
        modelAura = glm::translate(modelAura, exp.position);
        modelAura = glm::scale(modelAura, glm::vec3(auraCurrent));

        shader.setMat4("model", modelAura);
        shader.setVec3("customColor", glm::vec3(0.5f, 0.0f, 1.0f)); 
        shader.setFloat("alpha", exp.alpha * 0.4f);
        
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);

        // [CORE EXPLOSION SPHERE]
        float coreStart = exp.maxSize * 0.15f;
        float coreEnd   = exp.maxSize * 0.40f;
        float coreCurrent = glm::mix(coreStart, coreEnd, growthFactor);

        glm::mat4 modelCore = glm::mat4(1.0f);
        modelCore = glm::translate(modelCore, exp.position);
        modelCore = glm::scale(modelCore, glm::vec3(coreCurrent));

        shader.setMat4("model", modelCore);
        shader.setVec3("customColor", glm::vec3(1.0f, 1.0f, 1.0f)); 
        shader.setFloat("alpha", exp.alpha); 
        
        glDrawElements(GL_TRIANGLES, sphereIndexCount, GL_UNSIGNED_INT, 0);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    shader.setBool("isSun", false);
    shader.setBool("useColor", false);
}

//* Dynamic Asteroids
void drawDynamicAsteroids(Shader &shader, glm::mat4 projection, glm::mat4 view) {
    shader.use();
    shader.setBool("isSun", false); 
    shader.setBool("isRing", false);

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, asteroidTexture);
    shader.setInt("diffuseMap", 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    shader.setInt("normalMap", 1);

    glBindVertexArray(sphereVAO);

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
    shader.setBool("useColor", true);
    shader.setBool("isRing", false);

    shader.setMat4("projection", glm::mat4(1.0f));
    shader.setMat4("view", glm::mat4(1.0f));
    shader.setMat4("model", glm::mat4(1.0f));

    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, flatNormalMap); shader.setInt("diffuseMap", 0);

    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_LINES, 0, 4);

    glEnable(GL_DEPTH_TEST);
    shader.setBool("useColor", false);
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

    const int windowWidth = 1280;
    const int windowHeight = 720;
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "OpenGL_Space-Shooter", NULL, NULL);
    

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

//* Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

//* Initialize IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui::StyleColorsDark(); 
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

//* Set Viewport & Configurations
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    glEnable(GL_DEPTH_TEST);


/***********************************************
** SECTION 8: ASSETS LOADING & GEOMETRY SETUP **
***********************************************/

//* Setup Geometry Models
    setupSphere();
    setupRing();
    setupShip();
    setupCrosshair();

//* Load Shaders
    Shader mainShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    mainShader.use();
    glUniform1i(glGetUniformLocation(mainShader.ID, "texture_diffuse1"), 0);

//* Load Textures
    universeTexture = loadTexture("textures/8k_stars_milky_way.jpg", false);
    sunTexture = loadTexture("textures/8k_sun.jpg", false);
    mercuryTexture = loadTexture("textures/8k_mercury.jpg", false);
    venusTexture = loadTexture("textures/8k_venus_surface.jpg", false);
    earthTexture = loadTexture("textures/8k_earth_daymap.jpg", false);
    moonTexture = loadTexture("textures/8k_moon.jpg", false);
    marsTexture = loadTexture("textures/8k_mars.jpg", false);
    jupiterTexture = loadTexture("textures/8k_jupiter.jpg", false);
    saturnTexture = loadTexture("textures/8k_saturn.jpg", false);
    saturnRingTexture = loadTexture("textures/8k_saturn_ring.png", true);
    uranusTexture = loadTexture("textures/2k_uranus.jpg", false);
    neptuneTexture = loadTexture("textures/2k_neptune.jpg", false);
    shipTexture = loadTexture("textures/spaceship.jpg", false);
    asteroidTexture = loadTexture("textures/4k_makemake.jpg", false);

//* Load Normal Maps
    mercuryNormalMap = loadTexture("textures/mercury_normal.png", false);
    venusNormalMap = loadTexture("textures/venus_normal.png", false);
    earthNormalMap = loadTexture("textures/earth_normal.png", false);
    moonNormalMap = loadTexture("textures/moon_normal.png", false);
    marsNormalMap = loadTexture("textures/mars_normal.png", false);
    
    // Generate Default Uniform NormalMap
    glGenTextures(1, &flatNormalMap);
    glBindTexture(GL_TEXTURE_2D, flatNormalMap);
    unsigned char pixel[] = { 128, 128, 255, 255 }; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Init Random seed
    std::srand(static_cast<unsigned int>(std::time(0)));
    gameStartTime = (float)glfwGetTime();

       
/************************************
** SECTION 11: FRAME LOGIC & CLEAR **
************************************/
    while (!glfwWindowShouldClose(window)) {

        // START IMGUI FRAME
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // UI Box Definition
        ImGui::Begin("Mission Control"); 
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Score: %d", gameScore);
        ImGui::Text("Survival Time: %.1fs", survivalTime);
        ImGui::End();

    //* Delta Time    
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        survivalTime = (float)glfwGetTime() - gameStartTime;

    //* Updated Planet Positions
        mercuryPos = calculateOrbit(sunPos, mercuryOrbitRadius, mercuryOrbitSpeed, currentFrame);
        venusPos = calculateOrbit(sunPos, venusOrbitRadius, venusOrbitSpeed, currentFrame);
        earthPos = calculateOrbit(sunPos, earthOrbitRadius, earthOrbitSpeed, currentFrame);
        moonPos = calculateOrbit(earthPos, moonOrbitRadius, moonOrbitSpeed, currentFrame);
        marsPos = calculateOrbit(sunPos, marsOrbitRadius, marsOrbitSpeed, currentFrame);
        jupiterPos = calculateOrbit(sunPos, jupiterOrbitRadius, jupiterOrbitSpeed, currentFrame);
        saturnPos = calculateOrbit(sunPos, saturnOrbitRadius, saturnOrbitSpeed, currentFrame);
        uranusPos = calculateOrbit(sunPos, uranusOrbitRadius, uranusOrbitSpeed, currentFrame);
        neptunePos = calculateOrbit(sunPos, neptuneOrbitRadius, neptuneOrbitSpeed, currentFrame);

    //* Gameplay
        if (!isGameOver) {

        //* Spawn Asteroids
            if (currentFrame - lastSpawnTime > 2.0f) {
                spawnAsteroid();
                lastSpawnTime = currentFrame;
            }

        //* Update Asteroids 
            for (auto &ast : asteroids) {
                if (!ast.isActive) continue; 

            // Asteroids trajectories
                glm::vec3 currentDir = glm::normalize(earthPos - ast.position);
                float currentSpeed = glm::length(ast.velocity); 
                if (currentSpeed < minAsteroidSpeed) currentSpeed = minAsteroidSpeed;
                ast.velocity = currentDir * currentSpeed;
                ast.position += ast.velocity * deltaTime;

            // Check Earth collision -> Endgame effect
                float distToEarth = glm::distance(earthPos, ast.position);
                if (distToEarth < (earthScale * 0.5f) + (ast.scale * 0.5f)) {
                    earthIsHit = true; 
                    isGameOver = true;
                }

            // Destroying Asteroids on collision with spaceship 
                float distToShip = glm::distance(cameraPos, ast.position);
                if (distToShip < (ast.scale + 10.0f)) { 
                    Explosion exp;
                    exp.position = ast.position;
                    exp.size = 1.0f;               
                    exp.maxSize = ast.scale * 4.0f; 
                    exp.alpha = 1.0f;
                    exp.lifeTime = 0.4f;
                    activeExplosions.push_back(exp);
                    ast.isActive = false; 
                }   
            
            }
        
        //* Clean Up inactive asteroids
            asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(), [](const Asteroid& a) { 
                return !a.isActive; 
            }), asteroids.end());

        //* Update Lasers & Check Target Hits
            for (int i = 0; i < activeLasers.size(); i++) {
                activeLasers[i].trail.push_back(activeLasers[i].position);
                if (activeLasers[i].trail.size() > 15) { 
                    activeLasers[i].trail.erase(activeLasers[i].trail.begin());
                }

                activeLasers[i].position += activeLasers[i].direction * activeLasers[i].speed * deltaTime;
                activeLasers[i].lifeTime -= deltaTime;

                for (auto &ast : asteroids) {
                    if (!ast.isActive) continue;

                    float dist = glm::distance(activeLasers[i].position, ast.position);
                    if (dist < (ast.scale * 2.5f)) {
                        destroyedAsteroids++;
                        float distDePamant = glm::distance(ast.position, earthPos);
                        float distNormalized = distDePamant / spawnDistance; 
                        if (distNormalized > 1.0f) distNormalized = 1.0f;
                        float proximityBonus = 1.0f - distNormalized;
                        float asteroidBonus = 100.0f + (200.0f * proximityBonus);
                        gameScore += asteroidBonus;
                        ast.isActive = false;           
                        activeLasers[i].lifeTime = 0;   

                        Explosion exp;
                        exp.position = ast.position;
                        exp.size = 1.0f;               
                        exp.maxSize = ast.scale * 5.0f; 
                        exp.alpha = 1.0f;
                        exp.lifeTime = 0.4f;
                        activeExplosions.push_back(exp);
                        break; 
                    }
                }

                if (activeLasers[i].lifeTime <= 0) {
                    activeLasers.erase(activeLasers.begin() + i);
                    i--;
                }
            }

            for (int i = 0; i < activeExplosions.size(); i++) {
                if (activeExplosions[i].size < activeExplosions[i].maxSize) {
                    float diff = activeExplosions[i].maxSize - activeExplosions[i].size;
                    activeExplosions[i].size += diff * 15.0f * deltaTime; 
                }
    
                activeExplosions[i].alpha -= 2.0f * deltaTime;
                activeExplosions[i].lifeTime -= deltaTime;
    
                if (activeExplosions[i].lifeTime <= 0 || activeExplosions[i].alpha <= 0) {
                    activeExplosions.erase(activeExplosions.begin() + i);
                    i--;
                }
            }
        }

    //* Process Input

        shipRoll = glm::mix(shipRoll, -mouseXOffset * 5.0f, deltaTime * 3.0f);
        shipPitch = glm::mix(shipPitch, mouseYOffset * 5.0f, deltaTime * 3.0f);
        mouseXOffset = glm::mix(mouseXOffset, 0.0f, deltaTime * 2.0f);
        mouseYOffset = glm::mix(mouseYOffset, 0.0f, deltaTime * 2.0f);
        
        processInput(window);

        // Check Ship-Planets Collisions
        std::vector<PlanetCollision> solidObjects = {
            {sunPos, sunScale},
            {mercuryPos, mercuryScale},
            {venusPos, venusScale},
            {earthPos, earthScale},
            {marsPos, marsScale},
            {jupiterPos, jupiterScale},
            {saturnPos, saturnScale},
            {uranusPos, uranusScale},
            {neptunePos, neptuneScale}
        };  

        for (const auto& planet : solidObjects) {
            float distance = glm::distance(cameraPos, planet.position);
            float minDistance = planet.radius + 15.0f; 

            if (distance < minDistance) {
                glm::vec3 collisionNormal = glm::normalize(cameraPos - planet.position);
                cameraPos = planet.position + collisionNormal * minDistance;
            }
        }
    

    //* Clear Buffers
        glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //* Window's Aspect Ratio
        int currentWidth, currentHeight;
        glfwGetFramebufferSize(window, &currentWidth, &currentHeight);
        float aspect = (float)currentWidth / (float)currentHeight;

    //* Projection & View Matrices
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 10000.0f);
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

        drawDynamicAsteroids(mainShader, projection, view);
        drawLaser(mainShader, projection, view);
        drawExplosions(mainShader, projection, view);

        glClear(GL_DEPTH_BUFFER_BIT);
        drawShip(mainShader, projection);
        drawCrosshair(mainShader);


        if (earthIsHit) {
            flashAlpha += deltaTime * flashSpeed;
    
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_DEPTH_TEST); 
    
            ImGui::GetForegroundDrawList()->AddRectFilled(
                ImVec2(0, 0), 
                ImVec2((float)currentWidth, (float)currentHeight), 
                ImColor(1.0f, 1.0f, 1.0f, flashAlpha)
            );
    
            if (flashAlpha >= 1.0f) {
                glfwSetWindowShouldClose(window, true); 
            }
        }

//* Draw IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

//* BUFFER SWAP & EVENT POLLING
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

/************************
** SECTION 0: CLEAN UP **
************************/
    float timePoints = survivalTime * 10.0f;
    int finalScore = static_cast<int>(gameScore + timePoints);

    std::cout << "\n===============================" << std::endl;
    std::cout << "       MISSION TERMINATED!     " << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "Final Score:       " << finalScore << " pts" << std::endl;
    std::cout << "Survival:          " << (int)survivalTime << "s" << std::endl;
    std::cout << "Targets destroyed: " << destroyedAsteroids << std::endl;
    std::cout << "===============================\n" << std::endl;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}