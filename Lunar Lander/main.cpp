#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

//want these amount of  tiles
#define PLATFORM_COUNT 3
#define FLOOR_COUNT 20

struct GameState {
    Entity player;
	Entity platforms[PLATFORM_COUNT];
	Entity floor[FLOOR_COUNT];
	Entity text;
	
};

GameState state;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);
    
    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(image);
    return textureID;
}




void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Lunar Lander!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
	//load the fonts
	state.text.position = glm::vec3(0);
	state.text.width = 2;
	state.text.height = 2;



	//start higher up
	state.player.position = glm::vec3(0, 3, 0);
	//set player acceleration as gravity
	state.player.acceleration = glm::vec3(0, -1.6, 0);
    state.player.textureID = LoadTexture("rocket.png");
    

	//change the player hitbox, make me "skinnier"
	state.player.width = 0.5f;

	state.player.entityType = ROCKET;
	for (int i = 0; i < PLATFORM_COUNT; i++) {
		state.platforms[i].entityType = PLATFORM;
	}
	for (int i = 0; i < FLOOR_COUNT; i++) {
		state.floor[i].entityType = FLOOR;
	}
	//state.floor.entityType = EntityType(FLOOR);
	//state.platforms.entityType = PLATFORM;


	//load platform into game ground
	GLuint platformTile = LoadTexture("platform.png");
	for (int i = 0; i < 3; i++) {
		state.platforms[i].textureID = platformTile;
		state.platforms[i].position = glm::vec3(i-1, -3.25, 0);
	}
	//load the rocks into the game
	GLuint tileTextureID = LoadTexture("space_floor.png");
	/**/
	//load rocks the left side of the screen
	for (int i = 0; i < 7; i++) {
		state.floor[i].textureID = tileTextureID;
		state.floor[i].position = glm::vec3(-4.5, i-3.25, 0);
	}

	//load rocks the right side of the screen
	for (int i = 7; i < 14; i++) {
		state.floor[i].textureID = tileTextureID;
		state.floor[i].position = glm::vec3(4.5, i-10.25, 0);
	}

	//load rocks onto the bottom left
	for (int i = 14; i < 16; i++) {
		state.floor[i].textureID = tileTextureID;
		state.floor[i].position = glm::vec3(i-17, -3.25, 0);
	}

	//load rocks onto bottom right
	for (int i = 16; i < 18; i++) {
		state.floor[i].textureID = tileTextureID;
		state.floor[i].position = glm::vec3(i-14, -3.25, 0);
	}

	//add random blocks
	state.floor[18].textureID = tileTextureID;
	state.floor[18].position = glm::vec3(2, 2, 0);

	state.floor[19].textureID = tileTextureID;
	state.floor[19].position = glm::vec3(-2, 0, 0);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        // Some sort of action
						state.player.velocity.y = 2.0f;
                        break;
                        
                }
                break;
        }
    }
	state.player.velocity.x = 0;
	// Check for pressed/held keys below
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_A]) {
		state.player.velocity.x = -3;
		//state.player.acceleration *= 1.2;

	}
	else if (keys[SDL_SCANCODE_D]) {
		state.player.velocity.x = 3;
		//state.player.acceleration *= 1.2;

	}


}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
		//adding platforms and count for check collision?? 
        state.player.Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
		state.player.Update(FIXED_TIMESTEP, state.floor, FLOOR_COUNT);

		if (state.player.lastCollison == FLOOR) {
			state.text.textureID = LoadTexture("failed.png");
			//break;
		}
		else if (state.player.lastCollison == PLATFORM) {
			state.text.textureID = LoadTexture("success   .png");
			//break;
		}
        
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;

	
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
	//for loop to render all tiles
	for (int i = 0; i < PLATFORM_COUNT; i++) {
		state.platforms[i].Render(&program);
	}

	for (int i = 0; i < FLOOR_COUNT; i++) {
		state.floor[i].Render(&program);
	}

    state.player.Render(&program);
	state.text.Render(&program);
	
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
