#include "Shape.h"
#include <iostream>
#include <vector>
#include <limits>

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
    for (auto vertex : vertices){
        localVertices.push_back(vertex);
        worldVertices.push_back(vertex);
    }
    std::cout << "PolygonShape constructor called!" << std::endl;
}

PolygonShape::~PolygonShape() {
    std::cout << "PolygonShape destructor called!" << std::endl;
}

ShapeType PolygonShape::GetType() const {
    return POLYGON;
}

Shape* PolygonShape::Clone() const {
    return new PolygonShape(localVertices); // 使用复制构造函数创建一个新的PolygonShape对象，并返回其指针
}

float PolygonShape::GetMomentOfInertia() const {
    // 计算多边形的转动惯量，使用多边形的顶点信息进行计算
    // 这里可以使用多边形的面积和质心来计算转动惯量，或者使用其他方法
    return 5000; // 这里暂时返回0，实际实现需要根据多边形的形状进行计算
}

Vec2 PolygonShape::EdgeAt(int index) const{
    int currVertex = index;
    int nextVertex = (index + 1) % worldVertices.size();
    return worldVertices[nextVertex] - worldVertices[currVertex];
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position) {
    // 更新多边形的顶点位置，根据旋转角度和位置进行变换
    for (int i = 0; i < localVertices.size(); i++){
        worldVertices[i] = localVertices[i].Rotate(angle);
        worldVertices[i] += position;
    }
}

float PolygonShape::FindMinSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const{
    float separation = std::numeric_limits<float>::lowest();

    for (int i = 0; i < this->worldVertices.size(); i++){
        Vec2 va = this->worldVertices[i];
        Vec2 normal = this->EdgeAt(i).Normal();

        float minSep = std::numeric_limits<float>::max();
        Vec2 minVertex;
        for (int j = 0; j < other->worldVertices.size(); j++){
            Vec2 vb = other->worldVertices[j];
            float proj = (vb - va).Dot(normal);
            if (proj < minSep){
                minSep = proj;
                minVertex = vb;
            }
        }

        if (minSep > separation){
            separation = minSep;
            axis = this->EdgeAt(i); //找到进入边的法向量
            point = minVertex;   //找到进入的点
        }
    }
    return separation;
}

BoxShape::BoxShape(const float width, const float height) {
    this->width = width;
    this->height = height;
    

    localVertices.push_back(Vec2(-width / 2, -height / 2)); 
    localVertices.push_back(Vec2(-width / 2, height / 2)); 
    localVertices.push_back(Vec2(width / 2, height / 2)); 
    localVertices.push_back(Vec2(width / 2, -height / 2));

    worldVertices.push_back(Vec2(-width / 2, -height / 2)); 
    worldVertices.push_back(Vec2(-width / 2, height / 2)); 
    worldVertices.push_back(Vec2(width / 2, height / 2)); 
    worldVertices.push_back(Vec2(width / 2, -height / 2)); 
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
