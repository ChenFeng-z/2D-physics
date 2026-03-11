#include "Body.h"
#include "Vec2.h"
#include "Contact.h"
#include <iostream>

void Contact:: ResolvePenetration(){
    if (a -> IsStatic() && b -> IsStatic()){
        return;
    }
    float da = depth / (a -> inverseMass + b -> inverseMass) * a -> inverseMass;
    float db = depth / (a -> inverseMass + b -> inverseMass) * b -> inverseMass;

    a -> position -= normal * da;
    b -> position += normal * db; 
}

void Contact::ResolveCollision(){
    ResolvePenetration();

    float e = std::min(a->restitution, b->restitution);

    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

    const Vec2 vrel = va - vb;
    
    float vrelDotNormal = vrel.Dot(normal);

    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + e) * vrelDotNormal / ((a -> inverseMass + b -> inverseMass) + ra.Cross(normal)*ra.Cross(normal)*a->invI + rb.Cross(normal)*rb.Cross(normal)*b->invI);

    Vec2 jn = impulseDirection * impulseMagnitude;

    a -> ApplyImpulse(jn, ra);
    b -> ApplyImpulse(-jn, rb);
}