#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <vector>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();  // 系统调用，通过Graphics类打开窗口并返回是否成功打开的结果

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2, Graphics::Height() - 50, 0.0);
    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25, 0.0);
    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
    floor -> restitution = 0.5;
    leftWall -> restitution = 0.2;
    rightWall -> restitution = 0.2;
    bodies.push_back(floor);
    bodies.push_back(leftWall);
    bodies.push_back(rightWall);
    Body* bigBox = new Body(BoxShape(200, 200), Graphics::Width() / 2, Graphics::Height() / 2, 0.0);
    bigBox->SetTexture("./assets/crate.png");
    bigBox->rotation = 1.4;
    bigBox->restitution = 0.7;
    bodies.push_back(bigBox); 
    
    Body* ball = new Body(CircleShape(50), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 1.0);
    ball->SetTexture("./assets/basketball.png");
    ball->restitution = 0.1;
    bodies.push_back(ball);
    
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
                if (event.key.keysym.sym == SDLK_d)
                    debug = !debug;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT){
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    Body* ball = new Body(CircleShape(30), x, y, 1.0);
                    ball->SetTexture("./assets/basketball.png");
                    ball->restitution = 0.5;
                    bodies.push_back(ball);
                }
                if (event.button.button == SDL_BUTTON_RIGHT){
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    Body* box = new Body(BoxShape(60, 60), x, y, 1.0);
                    box->SetTexture("./assets/crate.png");
                    box->restitution = 0.2;
                    bodies.push_back(box);
                break;
                }
        }
    }         
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    // TODO: update all objects in the scene
    Graphics::ClearScreen(0xFF056263);  // 清屏，使用指定的颜色（十六进制ARGB格式）
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
    
        //body->AddForce(pushForce); // 将推力作用于粒子
        //Vec2 drag = Force::GenerateDragForce(*body, 0.01); // 生成阻力，使用一个阻力系数（例如0.5）
        //body->AddForce(drag);

        Vec2 weight = Vec2(0, 9.8 * body->mass* PIXELS_PER_METER); //
        body->AddForce(weight);
        //float torque = 20;
        //body -> AddTorque(torque);

        //Vec2 wind = Vec2(2.0 * PIXELS_PER_METER, 0);
        //body->AddForce(wind);
    }


    for(auto body : bodies) {
        body -> Update(deltaTime);
    }

    for (int i = 0; i <= bodies.size() - 1; i++){
        for (int j = i + 1; j < bodies.size(); j ++){
            Body* a = bodies[i];
            Body* b = bodies[j];
            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)){
                contact.ResolveCollision();

                if (debug){
                    Graphics::DrawFillCircle(contact.start.x, contact.start.y, 5, 0xFFFF0000); // 在碰撞点绘制一个红色圆，表示碰撞发生的位置
                    Graphics::DrawFillCircle(contact.end.x, contact.end.y, 5, 0xFFFF0000); // 在碰撞点绘制一个红色圆，表示碰撞发生的位置
                    Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFF00FF); // 绘制一条红色线段，表示碰撞的接触点之间的连接
                    a -> isColliding = true;
                    b -> isColliding = true;
                }
            }
        }
    }

    
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    
    //Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF0B3C4D); // 绘制一个填充的矩形，表示液体区域，使用指定的颜色（十六进制ARGB格式）
    

    for (auto body : bodies) {

        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)body->shape; // 将粒子的形状转换为CircleShape类型，以便访问半径属性
            if (!debug && body->texture){
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2,body->rotation, body->texture);
            }else{
                Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation,0xFF00FF00);
            }
        }
        if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*)body->shape; // 将粒子的形状转换为BoxShape类型，以便访问宽度和高度属性
            if (!debug && body->texture){
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }else{
                Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, 0xFF00FF00);
            }
        }
        if (body->shape->GetType() == POLYGON){
            PolygonShape* polygonShape = (PolygonShape*) body->shape;
            if (!debug && body->texture){
                Graphics::DrawFillPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF00FF00);
            }else{
                Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF00FF00);
            }
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