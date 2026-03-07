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
        Vec2 mouseCursor; // 定义一个向量，表示鼠标光标的位置
        bool leftMouseButtonDown = false; // 定义一个布尔变量，表示左键是否按下
        SDL_Rect liquid; // 定义一个矩形，表示液体区域

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