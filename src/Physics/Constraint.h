#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "Body.h"
#include "VecN.h"
#include "MatMN.h"

class Constraint {
    public:
        Body* a;
        Body* b;

        Vec2 aPoint;  //a的局部锚点
        Vec2 bPoint;  //b的局部锚点

        virtual ~Constraint() = default;

        MatMN GetInvM() const;
        VecN GetVelocities() const;

        //virtual void Solve();
};

class JointConstraint: public Constraint{
    private:
        MatMN jacobian;
    
    public:
        JointConstraint();
        JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
        //void Solve() override;
};

class PentrationConstraint: public Constraint {
    MatMN jacobian;
};

#endif