//
// Created by zhenya on 10.02.2026.
//

#include "RigidBody.h"

#include "GameObject.h"
#include "config.h"
void RigidBody::update(float dt) {
    if (!obj) return;
    velY += gravity * dt;
    obj->y += static_cast<int>(velY * dt);
    if (obj->y + obj->height > GameConfig::ground_level) {
        obj->y = GameConfig::ground_level - obj->height;
        velY = 0;
    }
}
