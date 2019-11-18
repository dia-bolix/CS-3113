//
//  Start.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "Start.h"

void Start::Initialize() {
    state.nextLevel = -1;
}
void Start::Update(float deltaTime) {
    
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_RETURN])
    {
        state.nextLevel = 1; 
		//give the player 3 lives again
		state.player.lives = 3;
    }
}
void Start::Render(ShaderProgram *program) {
    GLuint fontTextureID;
    fontTextureID = Util::LoadTexture("font1.png");
    Util::DrawText(program, fontTextureID, "Menu", 1.0f, -0.5f, glm::vec3(4.5, -1, 0));
    Util::DrawText(program, fontTextureID, "Vampire Slayer!", 1.0f, -0.5f, glm::vec3(1.5, -3, 0));
    Util::DrawText(program, fontTextureID, "Press Enter to start", 0.9f, -0.5f, glm::vec3(1.0, -6, 0));

	
}
