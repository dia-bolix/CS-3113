#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include "Effects.h"
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"
#include "SDL_mixer.h"

// Add headers
#include "Scene.h"
#include "Start.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "WinScreen.h"
#include "GameOver.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint fontTextureID;

Scene *currentScene;
Scene *sceneList[6];

Mix_Music* music;
Mix_Chunk* jump_sound;

void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}

Effects *effects;
void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Platformer!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
	//audio
	int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	//music from https://soundimage.org/wp-content/uploads/2018/08/A-Thousand-Exotic-Places.mp3
	music = Mix_LoadMUS("background.mp3");
	//jump sound from https://www.noiseforfun.com/waves/action-and-game/NFF-jump.wav
	jump_sound = Mix_LoadWAV("jump.wav");

	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    glViewport(0, 0, 640, 480);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
   // program.Load("shaders/vertex_textured.glsl",
    //             "shaders/effects_textured.glsl");
    fontTextureID = Util::LoadTexture("font1.png");
    
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
    
    sceneList[0] = new Start();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
	sceneList[4] = new WinScreen();
	sceneList[5] = new GameOver();
    SwitchToScene(sceneList[0]);
    
    effects = new Effects(projectionMatrix, viewMatrix);
   // effects->Start(FADEIN, 0.75f);
   // effects->Start(FADEOUT, 0.75f);
   // effects->Start(GROW, 5.0f);
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
                        currentScene->state.player.Jump();
						Mix_PlayChannel(-1, jump_sound, 1);
						break;
                    case SDLK_k:
                        effects->Start(SHAKE, 1.0f);
                        break;
                }
                break;
        }
    }
    
    currentScene->state.player.velocity.x = 0;
    
    // Check for pressed/held keys below
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (keys[SDL_SCANCODE_LEFT])
    {
        currentScene->state.player.velocity.x = -3.0f;
    }
    else if  (keys[SDL_SCANCODE_RIGHT])
    {
        currentScene->state.player.velocity.x = 3.0f;
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

        currentScene->Update(FIXED_TIMESTEP);
        effects->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
    
    accumulator = deltaTime;
    
    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player.position.x > 5) {
        viewMatrix = glm::translate(viewMatrix,
                                    glm::vec3(-currentScene->state.player.position.x, 3.75, 0));
    } else {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }
    viewMatrix = glm::translate(viewMatrix, effects->viewTranslate);
}

void Render() {
    
    glUseProgram(program.programID);

    glClear(GL_COLOR_BUFFER_BIT);
    
    program.SetViewMatrix(viewMatrix);
    program.SetLightPosition(currentScene->state.player.position);
    
    program.SetLightPosition2(glm::vec3(10,-5,0));

    currentScene->Render(&program);
    
  //  Util::DrawText(&program, fontTextureID, "Vampire Slayer!", 1.0f, -0.5f, glm::vec3(0, 0, 0));
    
    effects->Render();
    
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
	Mix_FreeMusic(music);
	Mix_FreeChunk(jump_sound);
    SDL_Quit();

}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        if (currentScene->state.nextLevel >= 0) {
            SwitchToScene(sceneList[currentScene->state.nextLevel]);
        }
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
