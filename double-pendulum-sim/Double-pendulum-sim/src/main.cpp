#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "pixel_camera.h"
#include <string>
#include <iostream>
#include "pendulum.h"


int main(void)
{
    //dummy values.
    int screenWidth = 1920;
    int screenHeight = 1080;

    int windowWidth = 1920;
    int windowHeight = 1080;

    const int virtualScreenWidth = 320;
    const int virtualScreenHeight = 180;

    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "raylib - C++ Double Pendulum Simulation");

    screenWidth = GetMonitorWidth(0);
    screenHeight = GetMonitorHeight(0);
    SetWindowSize(screenWidth, screenHeight);
    ToggleFullscreen();

    float virtualRatio = (float)screenWidth / (float)virtualScreenWidth;

    PixelCamera* camera = new PixelCamera(screenWidth, screenHeight, virtualRatio);

    Font guiFont = LoadFontEx("fonts/roboto.ttf", 15, nullptr, 0);
    GuiLoadStyle("styles/gray.rgs");
    GuiSetFont(guiFont);

    Font gameFont = LoadFontEx("fonts/roboto.ttf", 20, nullptr, 0);

    float g = 9.81f;

    Pendulum firstPendulum;

    firstPendulum.cordLength = 45.0f;
    firstPendulum.angle = 90.0f * DEG2RAD;
    firstPendulum.mass = 3.0f;
    firstPendulum.radius = firstPendulum.mass;
    firstPendulum.color = BLUE;

    Pendulum secondPendulum;

    secondPendulum.cordLength = 45.0f;
    secondPendulum.angle = 60.0f * DEG2RAD;
    secondPendulum.mass = 3.0f;
    secondPendulum.radius = secondPendulum.mass;
    secondPendulum.color = RED;

    float centerRadius = 3.0f;
    Vector2 pivotPosition = { 160, 50 };

    bool paused = false;

    while (!WindowShouldClose())
    {

        //---------------------UPDATE-----------------------
        firstPendulum.radius = firstPendulum.mass;
        secondPendulum.radius = secondPendulum.mass;

        if (IsKeyPressed(KEY_P))
        {
            paused = !paused;
        }

        if (IsKeyPressed(KEY_R))
        {
            firstPendulum.angle = 0.0f;
            firstPendulum.velocity = 0.0f;
            firstPendulum.acceleration = 0.0f;

            secondPendulum.angle = 0.0f;
            secondPendulum.velocity = 0.0f;
            secondPendulum.acceleration = 0.0f;

            g = 9.81f;

            firstPendulum.mass = 3.0f;
            secondPendulum.mass = 3.0f;

            firstPendulum.cordLength = 45.0f;
            secondPendulum.cordLength = 45.0f;

            pivotPosition = { 160, 50 };

            firstPendulum.color = RED;
            secondPendulum.color = RED;
        }

        firstPendulum.position.x = firstPendulum.cordLength * sin(firstPendulum.angle) + pivotPosition.x;
        firstPendulum.position.y = firstPendulum.cordLength * cos(firstPendulum.angle) + pivotPosition.y;

        secondPendulum.position.x = firstPendulum.position.x + secondPendulum.cordLength * sin(secondPendulum.angle);
        secondPendulum.position.y = firstPendulum.position.y + secondPendulum.cordLength * cos(secondPendulum.angle);


        if (!paused)
        {

            firstPendulum.velocity += firstPendulum.acceleration;
            secondPendulum.velocity += secondPendulum.acceleration;

            firstPendulum.angle += firstPendulum.velocity;
            secondPendulum.angle += secondPendulum.velocity;

            float firstNumerator =
                -g *
                (2 * firstPendulum.mass + secondPendulum.mass) *
                sin(firstPendulum.angle) -
                secondPendulum.mass *
                g *
                sin(firstPendulum.angle - 2 * secondPendulum.angle)
                - 2
                * sin(firstPendulum.angle - secondPendulum.angle) *
                secondPendulum.mass *
                (secondPendulum.velocity * secondPendulum.velocity *
                    secondPendulum.cordLength +
                    firstPendulum.velocity * firstPendulum.velocity *
                    firstPendulum.cordLength *
                    cos(firstPendulum.angle - secondPendulum.angle)
                    );

            float firstDenominator =
                firstPendulum.cordLength *
                (2 *
                    firstPendulum.mass +
                    secondPendulum.mass -
                    secondPendulum.mass *
                    cos(2 *
                        firstPendulum.angle -
                        2 *
                        secondPendulum.angle)
                    );

            float secondNumerator =
                2 *
                sin(firstPendulum.angle - secondPendulum.angle) *
                (firstPendulum.velocity * firstPendulum.velocity *
                    firstPendulum.cordLength *
                    (firstPendulum.mass + secondPendulum.mass) +
                    g * (firstPendulum.mass + secondPendulum.mass) *
                    cos(firstPendulum.angle) +
                    secondPendulum.velocity * secondPendulum.velocity *
                    secondPendulum.cordLength *
                    secondPendulum.mass *
                    cos(firstPendulum.angle - secondPendulum.angle));

            float secondDenominator =
                secondPendulum.cordLength *
                (2 * firstPendulum.mass +
                    secondPendulum.mass -
                    secondPendulum.mass * cos(2 * firstPendulum.angle - 2 * secondPendulum.angle)
                    );

            firstPendulum.acceleration = (firstNumerator / firstDenominator) * GetFrameTime();
            secondPendulum.acceleration = (secondNumerator / secondDenominator) * GetFrameTime();

            firstPendulum.velocity *= 0.997f;
            secondPendulum.velocity *= 0.997f;
        }


        if (CheckCollisionPointCircle({ GetMousePosition().x / virtualRatio, GetMousePosition().y / virtualRatio }, firstPendulum.position, 30))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                firstPendulum.angle = -(atan2f(GetMousePosition().y / virtualRatio - pivotPosition.y, GetMousePosition().x / virtualRatio - pivotPosition.x) - (90 * DEG2RAD));
                firstPendulum.velocity = 0.0f;
                firstPendulum.acceleration = 0.0f;

                secondPendulum.velocity = 0.0f;
                secondPendulum.acceleration = 0.0f;
            }
        }
        else if (CheckCollisionPointCircle({ GetMousePosition().x / virtualRatio, GetMousePosition().y / virtualRatio }, secondPendulum.position, 30))
        {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                secondPendulum.angle = -(atan2f(GetMousePosition().y / virtualRatio - firstPendulum.position.y, GetMousePosition().x / virtualRatio - firstPendulum.position.x) - (90 * DEG2RAD));
                secondPendulum.velocity = 0.0f;
                secondPendulum.acceleration = 0.0f;

                firstPendulum.velocity = 0.0f;
                firstPendulum.acceleration = 0.0f;
            }
        }

        camera->Update();

        //---------------------DRAW-----------------------


        BeginDrawing();
        ClearBackground(debug::debug_color);

        camera->BeginWorldMode();
        
        DrawLineEx(pivotPosition, firstPendulum.position, 1.0f, DARKGRAY);
        DrawLineEx(firstPendulum.position, secondPendulum.position, 1.0f, DARKGRAY);

        DrawCircle(pivotPosition.x, pivotPosition.y, centerRadius, DARKGRAY);
        DrawCircle(firstPendulum.position.x, firstPendulum.position.y, firstPendulum.radius, firstPendulum.color);
        DrawCircle(secondPendulum.position.x, secondPendulum.position.y, secondPendulum.radius, secondPendulum.color);


        camera->EndWorldMode();

        camera->DrawWorld();

        DrawFPS(screenWidth - MeasureText("FPS 0000", 20), 10);

        DrawTextEx(gameFont, "P to pause", { (float)screenWidth - MeasureText("P to pause", 20), 40 }, 20, 2, RAYWHITE);
        DrawTextEx(gameFont, "R to reset", { (float)screenWidth - MeasureText("P to pause", 20), 60 }, 20, 2, RAYWHITE);
        DrawTextEx(gameFont, "ESC to quit", { (float)screenWidth + 6 - MeasureText("ESC to quit", 20), 80 }, 20, 2, RAYWHITE);

        firstPendulum.mass = GuiSlider({ 10, 10, 300, 20 }, nullptr, ("FIRST MASS: " + std::to_string(firstPendulum.mass)).c_str(), firstPendulum.mass, 1.0f, 10.0f);
        firstPendulum.cordLength = GuiSlider({ 10, 35, 300, 20 }, nullptr, ("FIRST CORD LENGTH: " + std::to_string(firstPendulum.cordLength)).c_str(), firstPendulum.cordLength, 5.0f, 80.0f);
        secondPendulum.mass = GuiSlider({ 10, 100, 300, 20 }, nullptr, ("SECOND MASS: " + std::to_string(secondPendulum.mass)).c_str(), secondPendulum.mass, 1.0f, 10.0f);
        secondPendulum.cordLength = GuiSlider({ 10, 125, 300, 20 }, nullptr, ("SECOND CORD LENGTH: " + std::to_string(secondPendulum.cordLength)).c_str(), secondPendulum.cordLength, 5.0f, 80.0f);
        g = GuiSlider({ 10, 190, 300, 20 }, nullptr, ("GRAVITY: " + std::to_string(g)).c_str(), g, 0.0f, 30.0f);

        pivotPosition.x = GuiSlider({ 10, 255, 300, 20 }, nullptr, ("PIVOT X POSITION: " + std::to_string(pivotPosition.x)).c_str(), pivotPosition.x, 0, 320);
        pivotPosition.y = GuiSlider({ 10, 280, 300, 20 }, nullptr, ("PIVOT Y POSITION: " + std::to_string(pivotPosition.y)).c_str(), pivotPosition.y, 0, 180);

        GuiDrawText("FIRST MASS COLOR", { 10, 320, 300, 20 }, GUI_TEXT_ALIGN_LEFT, RAYWHITE);
        firstPendulum.color = GuiColorPicker({ 10, 340, 300, 100 }, firstPendulum.color);

        GuiDrawText("SECOND MASS COLOR", { 10, 480, 300, 20 }, GUI_TEXT_ALIGN_LEFT, RAYWHITE);
        secondPendulum.color = GuiColorPicker({ 10, 500, 300, 100 }, secondPendulum.color);

        EndDrawing();
    }

    delete camera;

    CloseWindow();

    return 0;
}