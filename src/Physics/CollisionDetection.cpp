#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle){
        return IsCollidingCircleCircle(a, b ,contact);
    }
    return false;
}

bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position; // 计算两个圆心之间的向量
    const float radiusSum = aCircleShape->radius + bCircleShape->radius; // 计算两个圆的半径之和

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum); // 判断两个圆心之间的距离是否小于等于半径之和

    if (!isColliding) {
        return false; // 如果没有碰撞，直接返回false
    }
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalized();
    contact.start = b->position - contact.normal* bCircleShape->radius;
    contact.end = a->position + contact.normal* aCircleShape->radius;

    contact.depth = (contact.end - contact.start).Magnitude();

    return true; // 如果发生碰撞，返回true
    
}