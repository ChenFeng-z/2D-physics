#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius) {
    this->radius = radius;
    std::cout << "CircleShape constructor called!" << std::endl;
}

CircleShape::~CircleShape() {
    std::cout << "CircleShape destructor called!" << std::endl;
}

ShapeType CircleShape::GetType() const {
    return CIRCLE;
}

Shape* CircleShape::Clone() const {
    return new CircleShape(radius); // 使用复制构造函数创建一个新的CircleShape对象，并返回其指针
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices) {
    
}

PolygonShape::~PolygonShape() {
}

ShapeType PolygonShape::GetType() const {
    return POLYGON;
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(vertices); // 使用复制构造函数创建一个新的PolygonShape对象，并返回其指针
}

BoxShape::BoxShape(const float width, const float height) {
    
}
BoxShape::~BoxShape() {
}

ShapeType BoxShape::GetType() const {
    return BOX;
}

Shape* BoxShape::Clone() const {
    return new BoxShape(width, height); // 使用复制构造函数创建一个新的BoxShape对象，并返回其指针
}
