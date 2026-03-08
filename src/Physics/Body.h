#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"
#include "Shape.h"

struct Body
{
    
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;
    float mass;
    float inverseMass;

    Vec2 sumForces;

    float rotation;
    float angularVelocity;
    float angularAcceleration;

    Shape* shape = NULL;

    Body(const Shape& shape, float x, float y, float mass);
    ~Body();

    void AddForce(const Vec2& force);
    void ClearForces();

    void Integrate(float dt);
};
#endif