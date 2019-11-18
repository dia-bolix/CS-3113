//
//  Level3.hpp
//  SDLProject
//
//  Created by Mayra Piasta on 11/9/19.
//  Copyright © 2019 ctg. All rights reserved.
//

#include "Scene.h"

class Level3 : public Scene {
public:
    void Initialize() override;
    void Update(float deltaTime) override;
    void Render(ShaderProgram *program) override;
};


