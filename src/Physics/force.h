#ifndef FORCE_H
#define FORCE_H

#include "./Vec2.h"
#include "./Particle.h"

struct Force
{
    static Vec2 GenerateDrayForce(const Particle& Particle, float k);
};

#endif