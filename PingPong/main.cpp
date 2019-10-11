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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include "ballEntity.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameOver = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

float lastTicks = 0;

Entity leftpong;
Entity rightpong;
ballEntity ball;


GLuint fontTextureID;

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
    displayWindow = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    leftpong.textureID = LoadTexture("left_pong.png");
	leftpong.speed = 2;
	rightpong.textureID = LoadTexture("right_pong.png");
	rightpong.speed = 2;
	ball.textureID = LoadTexture("ball.png");
	ball.speed = 2;
	ball.movement.y = -.5;
	ball.movement.x = -1;

	//make pongs show up on the left/right side of the screens
	leftpong.position = glm::vec3(-5,0,0);
	rightpong.position = glm::vec3(5,0,0);
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
	//midnight blue background
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
}

void ProcessInput() {
    //set to 0 so paddles don't keep moving when key not pressed
	leftpong.movement = glm::vec3(0, 0, 0);
	rightpong.movement = glm::vec3(0, 0, 0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;

        }
    }
	//3.75 to check if on edge of screen
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_W] && leftpong.position.y <= 3.75 && !gameOver) {
		leftpong.movement.y = 1;
	} else if (keys[SDL_SCANCODE_S] && leftpong.position.y >= -3.75 && !gameOver) {
		leftpong.movement.y = -1;
	}
	if (keys[SDL_SCANCODE_UP] && rightpong.position.y <= 3.75 && !gameOver) {
		rightpong.movement.y = 1;
	} else if (keys[SDL_SCANCODE_DOWN] && rightpong.position.y >= -3.75 && !gameOver) {
		rightpong.movement.y = -1;
	}

}

//modify update for pong and ball
//if ball didn't collside and reached the edge stop moving
void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    leftpong.Update(deltaTime);
	rightpong.Update(deltaTime);
	
	//if (abs(ball.position.y) >= 3.5f) {
		
	//}

	//the ball will bounce if it touches the top or bottom of the screen, as well as the paddles
	if (ball.checkCollide(leftpong) || ball.checkCollide(rightpong) || abs(ball.position.y) >= 3.75f) {
		ball.bounce();
	}
	else if (abs(ball.position.x) >= 5.0f) {
		ball.movement = glm::vec3(0);
		gameOver = true;
	}
	
	ball.Update(deltaTime);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    leftpong.Render(&program);
	rightpong.Render(&program);
	ball.Render(&program);
 
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
