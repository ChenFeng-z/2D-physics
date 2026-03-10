#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "Contact.h"
#include "Body.h"

struct CollisionDetection
{
    static bool IsColliding(Body* a, Body* b, Contact& contact);
    static bool IsCollidingCircleCircle(Body* a, Body* b, Contact& contact);
    static bool IscollidingPolygonPolygon(Body* a, Body* b, Contact& contact);
    //static bool IscollidingPolygonCircle(Body* a, Body* b, Contact& contact);
};

#endif