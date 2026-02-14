//
// Created by zhenya on 10.02.2026.
//

#include "RigidBody.h"

#include "GameObject.h"
#include "config.h"
void RigidBody::update(float dt) {
    if (!obj) return;
    if (onGround) return;
    velocityY += gravity * dt;
    obj->x += static_cast<int>(velocityX * dt);
    obj->y += static_cast<int>(velocityY * dt);
    if (obj->y + obj->height > GameConfig::ground_level) {
        obj->y = GameConfig::ground_level - obj->height;
        velocityY = 0;
    }
}
