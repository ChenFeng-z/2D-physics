#include "CollisionDetection.h"
#include <iostream>
#include <math.h>
#include "Shape.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle){
        return IsCollidingCircleCircle(a, b ,contact);
    }
    if (aIsPolygon && bIsPolygon){
        return IscollidingPolygonPolygon(a, b, contact);
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

bool CollisionDetection::IscollidingPolygonPolygon(Body* a, Body* b, Contact& contact){
    const PolygonShape* aPolygonShape = (PolygonShape*) a -> shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b -> shape;
    Vec2 aAxis, bAxis;
    Vec2 aPoint, bPoint;
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);
    if (abSeparation >= 0){
        return false;
    }
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);
    if (baSeparation >= 0){
        return false;
    }

    contact.a = a;
    contact.b = b;

    if (abSeparation > baSeparation){
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = contact.start + contact.normal * contact.depth;
    }else{
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.end = bPoint;
        contact.start = bPoint - contact.normal * contact.depth;
    }
    return true;
}