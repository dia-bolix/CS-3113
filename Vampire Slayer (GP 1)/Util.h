//
//  Util.h
//  SDLProject
//
//  Created by Mayra Piasta on 10/30/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#pragma once
#define GL_SILENCE_DEPRECATION
#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
class Util {
public:
    static GLuint fontTextureID;
    static GLuint LoadTexture(const char* filePath);
    static void DrawText(ShaderProgram *program, int fontTextureID, std::string
                         text, float size, float spacing, glm::vec3 position);
};
