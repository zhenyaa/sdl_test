//
// Created by zhenya on 10.02.2026.
//

#ifndef SDL_TEST_RIGIDBODY_H
#define SDL_TEST_RIGIDBODY_H
class GameObject;

class RigidBody {
    protected:

    public:
        float gravity = 600.0f;
        float velY = 1.0f;
        GameObject* obj;
        RigidBody(GameObject* obj, float gravity=500.0f): obj(obj), gravity(gravity) {};
        void update(float tick);
};


#endif //SDL_TEST_RIGIDBODY_H
