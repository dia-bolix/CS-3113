#include "Scene.h"
class GameOver : public Scene {
public:
	void Initialize() override;
	void Update(float deltaTime) override;
	void Render(ShaderProgram* program) override;
};

