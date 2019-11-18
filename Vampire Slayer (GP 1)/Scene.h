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
#include "Util.h"
#include "Entity.h"
#include "Map.h"

#define ENEMY_COUNT 2

struct GameState {
    Entity player;
    Map *map;
    int nextLevel;
   // Entity enemies;
	Entity enemies[ENEMY_COUNT];
	Entity heart1;
	Entity heart2;
	Entity heart3;
};

class Scene {
public:
	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(ShaderProgram* program) = 0;
};