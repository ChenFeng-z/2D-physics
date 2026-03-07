#include "particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    this->inverseMass = (mass != 0) ? 1.0f / mass : 0.0f; // 计算质量的倒数，避免除以零
    std::cout << "Particle constructor called!" << std::endl;
}

Particle ::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}

void Particle::Integrate(float dt) {
    acceleration = sumForces * inverseMass; // 根据牛顿第二定律计算加速度

    velocity += acceleration * dt; // 更新粒子的速度，根据加速度和时间差计算新的速度
    position += velocity * dt; // 更新粒子的位置，根据速度和时间差计算新的位置

    ClearForces(); // 在每次积分后清空总力，以便下一次计算
}

void Particle::AddForce(const Vec2& force) {
    sumForces += force; // 将外力累加到总力上
}

void Particle::ClearForces() {
    sumForces = Vec2(0, 0); // 清空总力
}
