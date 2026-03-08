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

    Body* bob = new Body(CircleShape(50), Graphics::Width() / 2,Graphics::Height() / 2, 1.0); 
    bodies.push_back(bob); // 将新创建的粒子添加到粒子列表中
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
                    Body* newBody = new Body(x, y, 1.0); // 创建一个新的粒子对象，初始位置为鼠标点击的位置，质量为1
                    newBody->radius = 5; // 设置粒子的半径为5像素
                    bodies.push_back(newBody); // 将新创建的粒子添加到粒子列表中
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
                    int lastBody = NUM_PARTICLES - 1; // 获取最后一个粒子的索引
                    Vec2 impulseDirection = (bodies[lastBody] -> position - mouseCursor).UnitVector();
                    float impluseMagnitude = (bodies[lastBody] -> position - mouseCursor).Magnitude() *  5.0; // 根据鼠标点击位置与粒子位置的距离计算冲量的大小，乘以一个系数（例如10）来调整冲量的强度
                    bodies[lastBody] -> velocity += impulseDirection * impluseMagnitude; // 将计算得到的冲量应用于粒子的速度，改变粒子的运动状态
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
    
    for (auto body : bodies) {
    
        body->AddForce(pushForce); // 将推力作用于粒子
        Vec2 drag = Force::GenerateDragForce(*body, 0.01); // 生成阻力，使用一个阻力系数（例如0.5）
        body->AddForce(drag);

        Vec2 weight = Vec2(0, 9.8 * body->mass* PIXELS_PER_METER); //
        body->AddForce(weight);
        
    }


    for(auto body : bodies) {
        body->IntegrateLinear(deltaTime); // 更新粒子的位置和速度
        body->IntegrateAngular(deltaTime); // 更新粒子的旋转角度和角速度

        float torque = 20;
        body -> AddTorque(torque);
    }

    for(auto body : bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)body->shape; // 将粒子的形状转换为CircleShape类型，以便访问半径属性
            if (body->position.x - circleShape->radius <= 0){
                body->position.x = circleShape->radius; // 碰到左边界，调整位置
                body->velocity.x *= -0.9; // 反弹并减少速度
            } else if (body->position.x + circleShape->radius >= Graphics::Width()){
                body->position.x = Graphics::Width() - circleShape->radius; // 碰到右边界，调整位置
                body->velocity.x *= -0.9; // 反弹并减少速度
            }
            if (body->position.y - circleShape->radius <= 0){
                body->position.y = circleShape->radius; // 碰到上边界，调整位置
                body->velocity.y *= -0.9; // 反弹并减少速度
            } else if (body->position.y + circleShape->radius >= Graphics::Height()){
                body->position.y = Graphics::Height() - circleShape->radius; // 碰到下边界，调整位置
                body->velocity.y *= -0.9; // 反弹并减少速度
            }
    }

}
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    Graphics::ClearScreen(0xFF056263);  // 清屏，使用指定的颜色（十六进制ARGB格式）
    //Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF0B3C4D); // 绘制一个填充的矩形，表示液体区域，使用指定的颜色（十六进制ARGB格式）
    

    for (auto body : bodies) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)body->shape; // 将粒子的形状转换为CircleShape类型，以便访问半径属性
            Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation,0xFFEEBB00); // 在窗口中绘制一个填充的白色圆，圆心坐标为粒子的位置，半径为粒子的半径
        }else{

        }
        
    }

    /*默认常用
    for (auto body : bodies) {
        Graphics::DrawFillCircle(body->position.x,body->position.y, body->radius, 0xFFFFFFFF); // 在窗口中绘制一个填充的白色圆，圆心坐标为(200, 200)，半径为40像素
    }
    */
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    // TODO: destroy all objects in the scene
    for(auto body : bodies) {
        delete body; // 释放粒子对象的内存
    }
    Graphics::CloseWindow();
}