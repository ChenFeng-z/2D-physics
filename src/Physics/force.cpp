#include "./Force.h"

Vec2 Force::GenerateDrayForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0, 0);
    if (particle.velocity.MagnitudeSquared() > 0) {
        dragForce = particle.velocity.UnitVector() * -1; // 反向
        dragForce *= k * particle.velocity.MagnitudeSquared(); // Fd = -k * v^2
    }
    return dragForce;
}

Vec2 Force::GenerateFrictionForce(const Particle& particle, float k) {
    Vec2 frictionForce = Vec2(0, 0);
    if (particle.velocity.MagnitudeSquared() > 0) {
        frictionForce = particle.velocity.UnitVector() * -1; // 反向
        float fricationMagitude = k;
        frictionForce *= fricationMagitude; 
    }
    return frictionForce;
}