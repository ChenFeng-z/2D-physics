#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include "./Physics/World.h"
#include <vector>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();  // 系统调用，通过Graphics类打开窗口并返回是否成功打开的结果

    world = new World(-2.8);

    Body* bigBall = new Body(CircleShape(64), Graphics::Width() / 2.0, Graphics::Height() / 2.0, 0.0);
    bigBall->SetTexture("./assets/bowlingball.png");
    world->AddBody(bigBall);

    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25, 0.0);
    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
    floor->restitution = 0.7;
    leftWall->restitution = 0.2;
    rightWall->restitution = 0.2;
    world->AddBody(floor);
    world->AddBody(leftWall);
    world->AddBody(rightWall);
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
                    ball->restitution = 0.7;
                    world->AddBody(ball);
                }
                if (event.button.button == SDL_BUTTON_RIGHT){
                    int x,y;
                    SDL_GetMouseState(&x, &y);
                    Body* box = new Body(BoxShape(60, 60), x, y, 1.0);
                    box->SetTexture("./assets/crate.png");
                    box->restitution = 0.2;
                    world->AddBody(box);
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
    
    world -> Update(deltaTime);

    
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Render() {
    
    //Graphics::DrawFillRect(liquid.x + liquid.w / 2, liquid.y + liquid.h / 2, liquid.w, liquid.h, 0xFF0B3C4D); // 绘制一个填充的矩形，表示液体区域，使用指定的颜色（十六进制ARGB格式）
    for (auto joint: world->GetConstraints()) {
        const Vec2 pa = joint->a->LocalSpaceToWorldSpace(joint->aPoint);
        const Vec2 pb = joint->b->LocalSpaceToWorldSpace(joint->aPoint);
        Graphics::DrawLine(pa.x, pa.y, pb.x, pb.y, 0xFF555555);
    }

    for (auto body : world -> GetBodies()) {

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
    delete world;
    Graphics::CloseWindow();
}