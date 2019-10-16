#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

enum EntityType { ROCKET, PLATFORM, FLOOR };

class Entity {
public:

	EntityType entityType;
	EntityType lastCollison;

    glm::vec3 position;
    // glm::vec3 movement;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float speed;

	//needed for check collsion
	float width;
	float height;
    
    GLuint textureID;
    
    Entity();
    
	//this checks if a player collided with a tile
	bool CheckCollision(Entity other);
	void CheckCollisionsX(Entity* objects, int objectCount);
	void CheckCollisionsY(Entity* objects, int objectCount);


	void Update(float deltaTime, Entity* objects, int objectCount);
    void Render(ShaderProgram *program);

	void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position);

};



