#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/force.h"

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();  // 系统调用，通过Graphics类打开窗口并返回是否成功打开的结果

    anchor = Vec2(Graphics::Width() / 2, 30);

    Particle* bob = new Particle(Graphics::Width() / 2, Graphics::Height() / 2, 2.0);
    bob->radius = 10;
    particles.push_back(bob); 
    /*
    liquid.x = 0;
    liquid.y = Graphics::Height() / 2; // 将液体区域的y坐标设置为窗口高度的一半
    liquid.w = Graphics::Width();
    liquid.h = Graphics::Height() / 2; // 将液体区域的高度设置为窗口高度的一半
    */
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
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = - 50 * PIXELS_PER_METER; // 按下向上箭头键时，设置推力向量的y分量为一个负值，表示向上的推力
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 50 * PIXELS_PER_METER; // 按下向下箭头键时，设置推力向量的y分量为一个正值，表示向下的推力
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = - 50 * PIXELS_PER_METER; // 按下向左箭头键时，设置推力向量的x分量为一个负值，表示向左的推力
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 50 * PIXELS_PER_METER; // 按下向右箭头键时，设置推力向量的x分量为一个正值，表示向右的推力
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_UP)
                    pushForce.y = 0; // 松开向上箭头键时，设置推力向量的y分量为零，表示没有垂直方向的推力
                if (event.key.keysym.sym == SDLK_DOWN)
                    pushForce.y = 0; // 松开向下箭头键时，设置推力向量的y分量为零，表示没有垂直方向的推力
                if (event.key.keysym.sym == SDLK_LEFT)
                    pushForce.x = 0; // 松开向左箭头键时，设置推力向量的x分量为零，表示没有水平方向的推力
                if (event.key.keysym.sym == SDLK_RIGHT)
                    pushForce.x = 0; // 松开向右箭头键时，设置推力向量的x分量为零，表示没有水平方向的推力
                break;
            /*
                case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x,y;
                    SDL_GetMouseState(&x, &y); // 获取鼠标点击的位置坐标
                    Particle* newParticle = new Particle(x, y, 1.0); // 创建一个新的粒子对象，初始位置为鼠标点击的位置，质量为1
                    newParticle->radius = 5; // 设置粒子的半径为5像素
                    particles.push_back(newParticle); // 将新创建的粒子添加到粒子列表中
                }
            */
            case SDL_MOUSEMOTION:
                mouseCursor.x = event.motion.x; // 更新鼠标光标的x坐标
                mouseCursor.y = event.motion.y; // 更新鼠标光标的y坐标
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = true; // 设置左键按下的状态为true
                    int x, y;
                    SDL_GetMouseState(&x, &y); // 获取鼠标点击的位置坐标
                    mouseCursor.x = x; // 更新鼠标光标的x坐标
                    mouseCursor.y = y; // 更新鼠标光标的y坐标
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (leftMouseButtonDown && event.button.button == SDL_BUTTON_LEFT) {
                    leftMouseButtonDown = false; // 设置左键按下的状态为false
                    Vec2 impulseDirection = (particles[0] -> position - mouseCursor).UnitVector();
                    float impluseMagnitude = (particles[0] -> position - mouseCursor).Magnitude() *  5.0; // 根据鼠标点击位置与粒子位置的距离计算冲量的大小，乘以一个系数（例如10）来调整冲量的强度
                    particles[0] -> velocity += impulseDirection * impluseMagnitude; // 将计算得到的冲量应用于粒子的速度，改变粒子的运动状态
        }
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
    
        particle->AddForce(pushForce); // 将推力作用于粒子
        Vec2 drag = Force::GenerateDragForce(*particle, 0.001); // 生成阻力，使用一个阻力系数（例如0.5）
        particle->AddForce(drag);

        Vec2 weight = Vec2(0, 9.8 * particle->mass) * PIXELS_PER_METER; 
        particle->AddForce(weight);
        
    }

    Vec2 springForce = Force::GenerateSpringForce(*particles[0], anchor, restLength, k);
    particles[0]->AddForce(springForce);

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
    //Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF0B3C4D); // 绘制一个填充的矩形，表示液体区域，使用指定的颜色（十六进制ARGB格式）
    

    if (leftMouseButtonDown) {
        Graphics::DrawLine(particles[0]->position.x, particles[0]->position.y, mouseCursor.x, mouseCursor.y, 0xFFFF0000); 
    }

    Graphics::DrawLine(anchor.x, anchor.y, particles[0]->position.x, particles[0]->position.y, 0xFF313131); // 绘制一条蓝色的线段，连接锚点和粒子的位置，表示弹簧的连接
    Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF001155); // 绘制一个填充的黄色圆，表示锚点，圆心坐标为anchor，半径为5像素
    Graphics::DrawFillCircle(particles[0]->position.x, particles[0]->position.y, particles[0]->radius, 0xFFFFFFFF); // 在窗口中绘制一个填充的白色圆，圆心坐标为粒子的位置，半径为粒子的半径
    
    /*默认常用
    for (auto particle : particles) {
        Graphics::DrawFillCircle(particle->position.x,particle->position.y, particle->radius, 0xFFFFFFFF); // 在窗口中绘制一个填充的白色圆，圆心坐标为(200, 200)，半径为40像素
    }
    */
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