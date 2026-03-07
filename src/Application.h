#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/Particle.h"
#include <vector>

class Application {
    private:
        bool running = false;
        std::vector<Particle*> particles; // 存储所有粒子的容器
        Vec2 pushForce = Vec2(0, 0); // 定义一个推力向量，初始值为零

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
        void Render();
        void Destroy();
};

#endif