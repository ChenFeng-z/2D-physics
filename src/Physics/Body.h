#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Body
{
    int radius;
    
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float mass;
    float inverseMass;

    Vec2 sumForces;

    Body(float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2& force);
    void ClearForces();

    void Integrate(float dt);
};
#endif