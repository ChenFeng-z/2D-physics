#include "./Force.h"

Vec2 Force::GenerateDrayForce(const Particle& particle, float k) {
    Vec2 dragForce = Vec2(0, 0);
    if (particle.velocity.MagnitudeSquared() > 0) {
        dragForce = particle.velocity.UnitVector() * -1; // 反向
        dragForce *= k * particle.velocity.MagnitudeSquared(); // Fd = -k * v^2
    }
    return dragForce;
}