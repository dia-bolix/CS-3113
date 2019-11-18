//
//  Level3.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 11/9/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "Level3.h"
//
//  Level2.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//


#define LEVEL3_WIDTH 18
#define LEVEL3_HEIGHT 8
#define ENEMY_COUNT 2

unsigned int level3_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3,
    0, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};


void Level3::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 4, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(5, 0, 0);
    state.player.acceleration = glm::vec3(0, -9.81f, 0);
    state.player.textureID = Util::LoadTexture("player.png");

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].entityType = ENEMY;
		state.enemies[i].isStatic = false;
		state.enemies[i].width = 1.0f;
		//state.enemies[i].position = glm::vec3(i, -2.25, 0);
		state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
		state.enemies[i].textureID = Util::LoadTexture("enemy.png");
	}

	state.enemies[0].position = glm::vec3(6, -2.25, 0);
	state.enemies[1].position = glm::vec3(14, -2.25, 0);
    
	//load hearts
	state.heart1.textureID = Util::LoadTexture("health_heart.png");
	state.heart1.position = glm::vec3(5, 0, 0);
	state.heart2.textureID = Util::LoadTexture("health_heart.png");
	state.heart2.position = glm::vec3(5, 0, 0);
	state.heart3.textureID = Util::LoadTexture("health_heart.png");
	state.heart3.position = glm::vec3(5, 0, 0);

    state.nextLevel = -1;
}
void Level3::Update(float deltaTime) {

    state.player.Update(deltaTime, NULL, 0, state.map);

	if (state.player.position.x >= 17) {
		state.nextLevel = 4;
	}

	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (state.enemies[i].entityType == ENEMY) {
			state.enemies[i].AI(state.player);
		}
		state.enemies[i].Update(deltaTime, NULL, 0, state.map);
	}

	//die if you run out of lives
	if (state.player.lives == 0) {
		state.nextLevel = 5;
	}

	//lose a life  if you fall off the screen
	if (state.player.position.y < -10) {
		state.player.lives -= 1;
		state.nextLevel = 1;
	}
	//if touched by ai 
	for (int i = 0; i < ENEMY_COUNT; i++) {
		if (state.player.CheckCollision(state.enemies[i])) {
			state.enemies[i].position.x -= 1;
			state.player.lives -= 1;
		}
	}

	//update the heart position to be right above the player
	state.heart1.position.x = state.player.position.x - 1;
	state.heart2.position.x = state.player.position.x;
	state.heart3.position.x = state.player.position.x + 1;
	state.heart1.position.y = state.player.position.y + 1;
	state.heart2.position.y = state.player.position.y + 1;
	state.heart3.position.y = state.player.position.y + 1;

}
void Level3::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);

	for (int i = 0; i < ENEMY_COUNT; i++) {
		state.enemies[i].Render(program);
	}

	//only render the amount of hearts left
	if (state.player.lives == 1) {
		state.heart1.Render(program);
	}
	else if (state.player.lives == 2) {
		state.heart1.Render(program);
		state.heart2.Render(program);
	}
	else if (state.player.lives == 3) {
		state.heart1.Render(program);
		state.heart2.Render(program);
		state.heart3.Render(program);
	}
}
