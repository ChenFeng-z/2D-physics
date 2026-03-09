#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "Body.h"

struct CollisionDecection
{
    static bool IsColliding(Body& a, Body& b);
    static bool IsCollidingCircleCircle(Body& a, Body& b);
    static bool IscollidingPolygonPolygon(Body& a, Body& b);
    static bool IscollidingPolygonCircle(Body& a, Body& b);
};

#endif