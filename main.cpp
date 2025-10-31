/**
 * Author: Perry Huang
 * Assignment: Rise of the AI
 * Date due: 2025-11-08, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

#include <raylib.h>

#include "./CS3113/Entity.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1600, SCREEN_HEIGHT = 900, FPS = 60, SIDES = 4;
constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;
constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0};

// Global Variables
AppStatus gAppStatus = RUNNING;

struct GameState {
    Entity* player;
    Entity* enemies;
    Map* map;
};

float gPreviousTicks = 0.0f;
float gTimeAccumulator = 0.0f;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lunar Lander");
    SetTargetFPS(FPS);
}

void processInput() {
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    float ticks = (float)GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP) {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();
}

void shutdown() {
    CloseWindow();  // Close window and OpenGL context
}

int main() {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
