//
// Created by zhenya on 13.02.2026.
//

#include "Collider.h"

bool Collider::collisionDetection(const GameObject* selfObj, const GameObject* otherObj) const {
    if (otherObj == nullptr) {return false;}
    for (auto& boxA : boxes)
    {
        float ax = selfObj->x + boxA.ox;
        float ay = selfObj->y + boxA.oy;
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
