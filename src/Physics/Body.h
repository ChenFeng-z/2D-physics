#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vec2.h"
#include "Shape.h"

struct Body
{
    bool isColliding = false;

    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float mass;
    float inverseMass;
    float I;
    float invI;

    float restitution;
    float friction;

    Vec2 sumForces;
    float sumTorque;

    float rotation;
    float angularVelocity;
    float angularAcceleration;

    Shape* shape = nullptr;

    SDL_Texture* texture = nullptr; //生成纹理，指向SDL库

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

    bool IsStatic() const;

    void SetTexture(const char* textureFileName);

    void ApplyImpulse(const Vec2& j);
    void ApplyImpulse(const Vec2& j, const Vec2& r);

    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearTorque();
    void ClearForces();

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    void Update(float dt);
};
#endif