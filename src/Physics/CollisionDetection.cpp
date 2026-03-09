#include "CollisionDetection.h"

bool CollisionDecection::IsColliding(Body* a, Body* b) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;

    if (aIsCircle && bIsCircle){
        return IsCollidingCircleCircle(a, b);
    }
}

bool CollisionDecection::IsCollidingCircleCircle(Body* a, Body* b) {
    CircleShape* aCircleShape = (CircleShape*) a->shape;
    CircleShape* bCircleShape = (CircleShape*) b->shape;

    const Vec2 ab = b->position - a->position; // 计算两个圆心之间的向量
    const float radiusSum = aCircleShape->radius + bCircleShape->radius; // 计算两个圆的半径之和

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum); // 判断两个圆心之间的距离是否小于等于半径之和
    return isColliding;
}