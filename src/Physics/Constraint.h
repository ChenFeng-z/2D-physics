#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "VecN.h"
#include "MatMN.h"

class Constraint {
    public:
        Body* a;
        Body* b;

        virtual ~Constraint() = default;

        MatMN GetInvM() const;
        VecN GetVelocities() const;

        virtual void Solve();
};

class DistanceConstraint: public Constraint{

};

class PentrationConstraint: public Constraint {

};

#endif