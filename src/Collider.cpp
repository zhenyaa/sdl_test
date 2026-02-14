//
// Created by zhenya on 13.02.2026.
//

#include "Collider.h"
#include "GameObject.h"
bool Collider::collisionDetection(const GameObject* obj, const GameObject* otherObj) {
    if (otherObj == nullptr && obj==nullptr) {return false;}
    const Collider* otherCollider = &otherObj->collider;
    for (auto& boxA : obj->collider.boxes)
    {
        float ax = obj->x + boxA.ox;
        float ay = obj->y + boxA.oy;
        float aw = boxA.w;
        float ah = boxA.h;

        for (auto& boxB : otherObj->collider.boxes)
        {
            float bx = otherObj->x + boxB.ox;
            float by = otherObj->y + boxB.oy;
            float bw = boxB.w;
            float bh = boxB.h;

            // Простая AABB проверка
            if (ax < bx + bw &&
                ax + aw > bx &&
                ay < by + bh &&
                ay + ah > by)
            {
                return true;
            }
        }
    }

    return false;
}
