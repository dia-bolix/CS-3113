#include "Entity.h"
#include <vector>
using namespace std;

Entity::Entity()
{
    position = glm::vec3(0);
    speed = 0;
	width = 1;
	height = 1;

}

void Entity::Update(float deltaTime, Entity* objects, int objectCount) {

	//keep accumulating velocity based on the acceleration
	velocity += acceleration * deltaTime;
	position.x += velocity.x * deltaTime;
	CheckCollisionsX(objects, objectCount);
	position.y += velocity.y * deltaTime;
	CheckCollisionsY(objects, objectCount);

	//check stuff in here and make font pop up?
	//if (objects->entityType == FLOOR && )


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


bool Entity::CheckCollision(Entity other) {
	float x_dist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
	float y_dist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);

	if (x_dist < 0 && y_dist < 0) {
		lastCollison = other.entityType;
		return true;
	}
	return false;

} 

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
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
			}
			else if (velocity.x < 0) {
				position.x += penetrationX;
				velocity.x = 0;
			}
		}
	}
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
	for (int i = 0; i < objectCount; i++)
	{
		Entity object = objects[i];
		if (CheckCollision(object))
		{
			float ydist = fabs(position.y - object.position.y);
			float penetrationY = fabs(ydist - (height / 2) - (object.height / 2));
			if (velocity.y > 0) {
				position.y -= penetrationY;
				velocity.y = 0;
			}
			else if (velocity.y < 0) {
				position.y += penetrationY;
				velocity.y = 0;
			}
		}
	}
}

void Entity::DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text, float size, float spacing, glm::vec3 position) {
	float width = 1.0f / 16.0f;
	float height = 1.0f / 16.0f;

	vector<float> vertices;
	vector<float> texCoords;

	for (int i = 0; i < text.size(); i++) {
		int index = (int)text[i];

		float u = (float)(index % 16) / 16.0f;
		float v = (float)(index / 16) / 16.0f;

		texCoords.insert(texCoords.end(), { u, v + height, u + width, v + height, u + width, v, u, v + height, u + width, v,u,v });

		float offset = (size + spacing) * i;
		vertices.insert(vertices.end(), { offset + (-0.5f * size), (-0.5f * size),
										offset + (0.5f * size), (-0.5f * size),
										offset + (0.5f * size), (0.5f * size),
										offset + (-0.5f * size), (-0.5f * size),
										offset + (0.5f * size), (0.5f * size),
										offset + (-0.5f * size), (0.5f * size) });
	}
}

