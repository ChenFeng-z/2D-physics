#include "Application.h"
#include "./Physics/Constants.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();  // 系统调用，通过Graphics类打开窗口并返回是否成功打开的结果

    Particle* smallball = new Particle(50, 100, 1.0); // 创建一个新的粒子对象，初始位置为(200, 200)，质量为1
    smallball->radius = 4; // 设置粒子的半径为4像素
    particles.push_back(smallball);

    Particle* bigball = new Particle(50,200, 3.0); // 创建另一个粒子对象
    bigball->radius = 12; // 设置第二个粒子的半径为12像素
    particles.push_back(bigball);
    // TODO: setup objects in the scene
}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // TODO: update all objects in the scene
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0) {
        SDL_Delay(timeToWait);
    }
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f; // 将时间差转换为秒
    if (deltaTime > 0.016f) { 
        deltaTime = 0.016f;
    }

    timePreviousFrame = SDL_GetTicks();
    
    for (auto particle : particles) {
        Vec2 Wind = Vec2(1.0 * PIXELS_PER_METER, 0); // 定义一个向右的风力向量
        particle->AddForce(Wind); // 将风力作用于粒子
    }

    for (auto particle : particles) {
        Vec2 weight = Vec2(0, 9.81 * PIXELS_PER_METER); // 定义一个向下的重力向量
        particle->AddForce(weight); // 将重力作用于粒子
    }

    for(auto particle : particles) {
        particle->Integrate(deltaTime); // 更新粒子的位置和速度
    }

    for(auto particle : particles) {
        if (particle->position.x - particle->radius <= 0){
            particle->position.x = particle->radius; // 碰到左边界，调整位置
            particle->velocity.x *= -0.9; // 反弹并减少速度
        } else if (particle->position.x + particle->radius >= Graphics::Width()){
            particle->position.x = Graphics::Width() - particle->radius; // 碰到右边界，调整位置
            particle->velocity.x *= -0.9; // 反弹并减少速度
        }
        if (particle->position.y - particle->radius <= 0){
            particle->position.y = particle->radius; // 碰到上边界，调整位置
            particle->velocity.y *= -0.9; // 反弹并减少速度
        } else if (particle->position.y + particle->radius >= Graphics::Height()){
            particle->position.y = Graphics::Height() - particle->radius; // 碰到下边界，调整位置
            particle->velocity.y *= -0.9; // 反弹并减少速度
        }
    }

}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);  // 清屏，使用指定的颜色（十六进制ARGB格式）
    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x,particle->position.y, particle->radius, 0xFFFFFFFF); // 在窗口中绘制一个填充的白色圆，圆心坐标为(200, 200)，半径为40像素
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    // TODO: destroy all objects in the scene
    for(auto particle : particles) {
        delete particle; // 释放粒子对象的内存
    }
    Graphics::CloseWindow();
}