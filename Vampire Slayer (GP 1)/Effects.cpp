//
//  Effects.cpp
//  SDLProject
//
//  Created by Mayra Piasta on 11/4/19.
//  Copyright © 2019 ctg. All rights reserved.
//
#include "Effects.h"

Effects::Effects(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    // Non textured Shader
    program.Load("shaders/vertex.glsl", "shaders/fragment.glsl");
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    currentEffect = NONE;
    alpha = 0;
    
    viewTranslate = glm::vec3(0,0,0);
}

void Effects::DrawOverlay()
{
    glUseProgram(program.programID);
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program.positionAttribute);
}

void Effects::Start(EffectType effectType, float effectSpeed)
{
    currentEffect = effectType;
    speed = effectSpeed;
    
    switch (currentEffect) {
        case NONE:
            break;
        case FADEIN:
            alpha = 1.0f;
            break;
        case FADEOUT:
            alpha = 0.0f;
            break;
            
        case GROW:
            size = 0.0f;
            alpha = 1.0f;
            break;
            
        case SHRINK:
            size = 10.0f;
            alpha = 1.0f;
            break;
        
        case SHAKE:
            timeLeft = 1.0f;
            break;
    }
}

void Effects::Update(float deltaTime)
{
    switch (currentEffect) {
        case NONE:
            break;
        case FADEIN:
            alpha -= deltaTime * speed;
            if (alpha <= 0) {
                currentEffect = NONE;
            }
            break;
        case FADEOUT:
            alpha += deltaTime * speed;
            if (alpha >= 1){
                currentEffect = NONE;
            }
            break;
        case GROW:
            size += deltaTime * speed;
            
            if (size >= 10){
                currentEffect = NONE;
            }
            break;
            
        case SHRINK:
            size -= deltaTime * speed;
            if (size >= 10){
                currentEffect = NONE;
            }
            break;
            
        case SHAKE:
            float max = 0.1f;
            float min = -0.1f;
            float r = ((float)rand() / RAND_MAX) * (max - min) + min;
            
            viewTranslate = glm::vec3(r,r,0);
            
            timeLeft -= deltaTime * speed;
            if (timeLeft <= 0){
                viewTranslate = glm::vec3(0,0,0);
                currentEffect = NONE;
            }
            break;
    }
}
            
void Effects::Render()
{
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    
    switch (currentEffect) {
        case NONE:
            return;
            break;
        case FADEIN:
        case FADEOUT:
            modelMatrix = glm::scale(modelMatrix, glm::vec3(10,10,0));
            program.SetModelMatrix(modelMatrix);
            program.SetColor(0,0,0,alpha);
            DrawOverlay();
            break;
        case SHRINK:
        case GROW:
            modelMatrix = glm::scale(modelMatrix, glm::vec3(size, size *0.75f, 0));
            program.SetModelMatrix(modelMatrix);
            program.SetColor(0,0,0,alpha);
            DrawOverlay();
            break;
        case SHAKE:
            break;
    }
}

