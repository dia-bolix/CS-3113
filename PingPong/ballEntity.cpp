#include "ballEntity.h"

ballEntity::ballEntity()
{
	position = glm::vec3(0);
	speed = 0;
	width = .5;
	height = .5;
}

void ballEntity::Update(float deltaTime)
{	
	//if on left or right edge of screen stop moving the ball
	//if (abs(position.y) >= 2.5f) {
		//movement.x = 0;
		//movement.y = 0;
	//}
	//else {
		position += movement * speed * deltaTime;
	//}
}


void ballEntity::Render(ShaderProgram* program) {
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, position);
	program->SetModelMatrix(modelMatrix);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
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


bool ballEntity::checkCollide(const Entity& other)
{
	float xdist = fabs(position.x - other.position.x) - ((width + other.width) / 2.0f);
	float ydist = fabs(position.y - other.position.y) - ((height + other.height) / 2.0f);
	if (xdist < 0 && ydist < 0)
	{
		return true;
	}
	return false;
}

void ballEntity::bounce() {
	movement.x *= -1;
	movement.y *= -1;
}
