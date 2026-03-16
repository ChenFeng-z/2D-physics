#include "../Graphics.h"
#include "body.h"
#include <iostream>
#include <math.h>

Body::Body(const Shape& shape, float x, float y, float mass) {
    this->shape = shape.Clone(); 
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0f;
    this->angularVelocity = 0.0f;
    this->angularAcceleration = 0.0f;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0f;
    this->restitution = 1.0;
    this->friction = 0.7;
    this->mass = mass;
    this->inverseMass = (mass != 0) ? 1.0f / mass : 0.0f; // 计算质量的倒数，避免除以零
    I = shape.GetMomentOfInertia() * mass; 
    this->invI = (I != 0) ? 1.0f / I : 0.0f;
    this->shape->UpdateVertices(rotation, position);
    std::cout << "Body constructor called!" << std::endl;
}

Body ::~Body() {
    delete shape; // 释放shape对象的内存
    SDL_DestroyTexture(texture);
    std::cout << "Body destructor called!" << std::endl;
}

void Body::SetTexture(const char* textureFileName){
    SDL_Surface* surface = IMG_Load(textureFileName);
    if (surface){
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

bool Body:: IsStatic() const{
    const float epsilon = 0.005f;
    return fabs(inverseMass - 0.0) < epsilon;
}

void Body::AddTorque(float torque) {
    sumTorque += torque; // 将外力矩累加到总力矩上
}

void Body::AddForce(const Vec2& force) {
    sumForces += force; // 将外力累加到总力上
}

void Body::ClearTorque() {
    sumTorque = 0.0f; // 清空总力矩
}

Vec2 Body::LocalSpaceToWorldSpace(const Vec2& point) const {
    Vec2 rotated = point.Rotate(rotation);
    return rotated + position;
}

Vec2 Body::WorldSpaceToLocalSpace(const Vec2& point) const {
    float translatedX = point.x - position.x;
    float translatedY = point.y - position.y;
    float rotatedX = cos(-rotation) * translatedX - sin(-rotation) * translatedY;
    float rotatedY = cos(-rotation) * translatedY + sin(-rotation) * translatedX;
    return Vec2(rotatedX, rotatedY);
}

void Body::ApplyImpulseLinear(const Vec2& j) {
    if (IsStatic())
        return;
    velocity += j * inverseMass;
}

void Body::ApplyImpulseAngular(const float j) {
    if (IsStatic())
        return;
    angularVelocity += j * invI;
}

void Body::ApplyImpulseAtPoint(const Vec2& j, const Vec2& r) {
    if (IsStatic())
        return;
    velocity += j * inverseMass;
    angularVelocity += r.Cross(j) * invI;
}

void Body::IntegrateForces(const float dt){
    if (IsStatic()){
        return;
    }
    acceleration = sumForces * inverseMass; // 根据牛顿第二定律计算加速度

    velocity += acceleration * dt; // 更新粒子的速度，根据加速度和时间差计算新的速度

    angularAcceleration = sumTorque * invI; // 根据牛顿第二定律计算角加速度

    angularVelocity += angularAcceleration * dt; // 更新粒子的角速度，根据角加速度和时间差计算新的角速度

    ClearForces();
    ClearTorque();
}

void Body::IntegrateVelocities(const float dt){
    if (IsStatic()){
        return;
    }
    position += velocity * dt;

    rotation += angularVelocity * dt;

    shape -> UpdateVertices(rotation, position);
}

void Body::ClearForces() {
    sumForces = Vec2(0, 0); // 清空总力
}

