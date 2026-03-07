#include "particle.h"
#include <iostream>

Particle::Particle(float x, float y, float mass) {
    this->position = Vec2(x, y);
    this->mass = mass;
    std::cout << "Particle constructor called!" << std::endl;
}

Particle ::~Particle() {
    std::cout << "Particle destructor called!" << std::endl;
}

void Particle::Integrate(float dt) {
    velocity += acceleration * dt; // 更新粒子的速度，根据加速度和时间差计算新的速度
    position += velocity * dt; // 更新粒子的位置，根据速度和时间差计算新的位置
}
