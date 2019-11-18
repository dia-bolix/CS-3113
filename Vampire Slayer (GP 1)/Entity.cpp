#include "Entity.h"

Entity::Entity()
{
    entityType = PLATFORM;
    isStatic = true;
    isActive = true;
    position = glm::vec3(0);
    speed = 0;
    width = 1;
    height = 1;
    count = 0;
	lives = 3;
}

bool Entity::CheckCollision(Entity other)
{
    if (isStatic == true) return false;
    if (isActive == false || other.isActive == false) return false;
    
    float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
    float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);

    if (xdist < 0 && ydist < 0)
    {
        
        return true;
    }
    
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++) {
        Entity object = objects[i];
        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object.position.y);
            float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}
void Entity::AIWalker(Entity player){
    switch (aiState){
        case IDLE:
            if (glm::distance(position, player.position) < 3.0f){
                aiState = WALKING;
            }
            break;
        case WALKING:
            if (player.position.x > position.x){
                velocity.x = 1.0f;
            }else {
                velocity.x = -1.0f;
            }
            break;
    }
}

void Entity::AI(Entity player){
    switch (aiType) {
        case WALKER:
            AIWalker(player);
            break;
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity object = objects[i];
        
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object.position.x);
            float penetrationX = fabs(xdist - (width / 2) - (object.width / 2));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
}


void Entity::Jump()
{
    if (collidedBottom)
    {
        velocity.y = 6.0f;
    }
}



void Entity::CheckCollisionsX(Map *map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }
    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::CheckCollisionsY(Map *map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);
    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);
    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
        if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
        else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
            position.y += penetration_y;
            velocity.y = 0;
            collidedBottom = true;
        }
    }


void Entity::Update(float deltaTime, Entity *objects, int objectCount, Map *map) {
    collidedTop = false; collidedBottom = false; collidedLeft = false; collidedRight = false;
    velocity += acceleration * deltaTime;
    position.y += velocity.y * deltaTime;
    CheckCollisionsY(map);
    CheckCollisionsX(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed
    position.x += velocity.x * deltaTime; // Move on X CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed
}



void Entity::Render(ShaderProgram *program) {
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

