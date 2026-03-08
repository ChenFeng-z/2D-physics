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

float CircleShape::GetMomentOfInertia() const {
    return 0.5f * radius * radius; // 圆形的转动惯量公式 I = 0.5 * m * r^2，其中 m 是质量，r 是半径
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

float PolygonShape::GetMomentOfInertia() const {
    // 计算多边形的转动惯量，使用多边形的顶点信息进行计算
    // 这里可以使用多边形的面积和质心来计算转动惯量，或者使用其他方法
    return 0.0f; // 这里暂时返回0，实际实现需要根据多边形的形状进行计算
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

float BoxShape::GetMomentOfInertia() const {
    return (1.0f / 12.0f) * (width * width + height * height); // 矩形的转动惯量公式 I = (1/12) * m * (w^2 + h^2)，其中 m 是质量，w 是宽度，h 是高度
}
