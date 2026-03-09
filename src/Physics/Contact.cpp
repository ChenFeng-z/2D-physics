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

    const Vec2 vrel = (a -> velocity - b -> velocity);
    
    float vrelDotNormal = vrel.Dot(normal);

    const Vec2 impulseDirection = normal;
    const float impulseMagnitude = -(1 + e) * vrelDotNormal / (a -> inverseMass + b -> inverseMass);

    Vec2 jn = impulseDirection * impulseMagnitude;

    a -> ApplyImpulse(jn);
    b -> ApplyImpulse(-jn);
}