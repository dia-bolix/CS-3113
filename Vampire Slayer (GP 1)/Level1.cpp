///
//  Level1.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8 

#define ENEMY_COUNT 2
//int ENEMY_COUNT = 2;

unsigned int level1_data[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3, 3, 3, 3,
    0, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};


void Level1::Initialize() {
    GLuint mapTextureID = Util::LoadTexture("tileset.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    state.player.entityType = PLAYER;
    state.player.isStatic = false;
    state.player.width = 1.0f;
    state.player.position = glm::vec3(5, 0, 0);
    state.player.acceleration = glm::vec3(0, -9.81f, 0);
    state.player.textureID = Util::LoadTexture("player.png");

    for (int i = 0; i < ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].isStatic = false;
        state.enemies[i].width = 1.0f;
        state.enemies[i].acceleration = glm::vec3(0, -9.81f, 0);
        state.enemies[i].textureID = Util::LoadTexture("enemy.png");
    }
	state.enemies[0].position = glm::vec3(1, -2.25, 0);
	state.enemies[1].position = glm::vec3(8, -2.25, 0);

   
	//load hearts
	state.heart1.textureID = Util::LoadTexture("health_heart.png");
	state.heart1.position = glm::vec3(5, 0, 0);
	state.heart2.textureID = Util::LoadTexture("health_heart.png");
	state.heart2.position = glm::vec3(5, 0, 0); 
	state.heart3.textureID = Util::LoadTexture("health_heart.png");
	state.heart3.position = glm::vec3(5, 0, 0);

    state.nextLevel = -1;
}
void Level1::Update(float deltaTime) {
    state.player.Update(deltaTime, NULL, 0, state.map);

	for (int i = 0; i < ENEMY_COUNT; i++){
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

    if (state.player.position.x >= 13) {
        state.nextLevel = 2;
    }

	//update the heart position to be right above the player
	state.heart1.position.x = state.player.position.x-1;
	state.heart2.position.x = state.player.position.x;
	state.heart3.position.x = state.player.position.x+1;
	state.heart1.position.y = state.player.position.y+1;
	state.heart2.position.y = state.player.position.y+1;
	state.heart3.position.y = state.player.position.y+1;

}
void Level1::Render(ShaderProgram *program) {
    state.map->Render(program);
    state.player.Render(program);

	for (int i = 0; i < ENEMY_COUNT; i++){
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
	else if (state.player.lives ==3){
		state.heart1.Render(program);
		state.heart2.Render(program);
		state.heart3.Render(program);
	}
	
}
