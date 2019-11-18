#include "Map.h"
#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Map.h"

enum  EntityType { PLAYER, PLATFORM, ENEMY };
enum AIState { IDLE, WALKING };

class Entity {
public:
    enum AIState aiState{IDLE};
    enum AIType {WALKER};
    AIType aiType;
    void AIWalker(Entity player);
    void AI(Entity player);

    EntityType entityType;
    bool isStatic;
    bool isActive;
    
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    
    float width;
    float height;
    float speed;
    
    int count;
	int lives;

    GLuint textureID;
    
    Entity();
    
    bool CheckCollision(Entity other);
    
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionsY(Entity *objects, int objectCount);
    
    void Update(float deltaTime, Entity *objects, int objectCount, Map *map);
    void Render(ShaderProgram *program);
    
    void Jump();
    
    bool collidedTop;
    bool collidedBottom;
    bool collidedLeft;
    bool collidedRight;
	bool offScreen;
    
    void CheckCollisionsX(Map *map);
    void CheckCollisionsY(Map *map);
};



