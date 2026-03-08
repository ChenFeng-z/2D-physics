#include "./Force.h"
#include <algorithm>

Vec2 Force::GenerateDragForce(const Particle& particle, float k) {
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

Vec2 Force::GenerateGravitationalForce(const Particle& a, const Particle& b, float G, float minDistance, float maxDistance) {
    Vec2 d = b.position - a.position; // 计算两个粒子之间的距离向量
    float distanceSquared = d.MagnitudeSquared();
    distanceSquared = std::clamp(distanceSquared, minDistance, maxDistance); // 限制距离的平方在最小和最大值之间，避免过大或过小导致的数值不稳定
    Vec2 attractionDirection = d.UnitVector(); // 计算引力的方向
    float attractionMagnitude = G * a.mass * b.mass / distanceSquared; 
    Vec2 attractionForce = attractionDirection * attractionMagnitude; // 计算引力的大小，使用万有引力公式 F = G * m1 * m2 / r^2
    return attractionForce;
}

Vec2 Force::GenerateSpringForce(const Particle& particle, Vec2 anchor, float restLength, float k) {
    Vec2 d = particle.position - anchor; // 计算粒子与锚点之间的距离向量
    float displacement = d.Magnitude() - restLength; // 计算弹簧的伸长量（当前距离 - 休息长度）
    Vec2 springDirection = d.UnitVector(); // 计算弹力的方向
    float springMagnitude = -k * displacement; // 计算弹力的大小，使用胡克定律 F = -k * x，其中 x 是弹簧的伸长量（当前距离 - 休息长度）
    Vec2 springForce = springDirection * springMagnitude; // 弹力的方向与伸长量相反
    return springForce;
}