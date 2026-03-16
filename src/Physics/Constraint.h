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

        virtual void PreSolve(const float dt) = 0;
        virtual void Solve() = 0;
};

class JointConstraint: public Constraint{
    private:
        MatMN jacobian;
        VecN cachedLambda;
        float bias;
    public:
        JointConstraint();
        JointConstraint(Body* a, Body* b, const Vec2& anchorPoint);
        void PreSolve(const float dt) override;
        void Solve() override;
};

class PentrationConstraint: public Constraint {
    private:
        MatMN jacobian;
        VecN cachedLambda;
        float bias;

        Vec2 normal;
    public:
        PentrationConstraint();
        PentrationConstraint(Body* a, Body* b, const Vec2& aCollisionPoint, const Vec2& bCollisionPoint, const Vec2& normal);
        void PreSolve(const float dt) override;
        void Solve() override;
};

#endif