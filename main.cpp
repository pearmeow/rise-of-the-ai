/**
 * Author: Perry Huang
 * Assignment: Rise of the AI
 * Date due: 2025-11-08, 11:59pm
 * I pledge that I have completed this assignment without
 * collaborating with anyone else, in conformance with the
 * NYU School of Engineering Policies and Procedures on
 * Academic Misconduct.
 */

// TODO: get a soundtrack
// TODO: get/create a main menu scene

#include "CS3113/LevelA.h"
#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 600, FPS = 120, NUMBER_OF_LEVELS = 1;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f, gTimeAccumulator = 0.0f;

Scene* gCurrentScene = nullptr;
std::vector<Scene*> gLevels = {};

LevelA* gLevelA = nullptr;

// Function Declarations
void switchToScene(Scene* scene);
void initialise();
void processInput();
void update();
void render();
void shutdown();

void switchToScene(Scene* scene) {
    gCurrentScene = scene;
    gCurrentScene->initialise();
}

void initialise() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Scenes");
    InitAudioDevice();

    gLevelA = new LevelA(ORIGIN, "#C0897E");

    gLevels.push_back(gLevelA);

    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() {
    gCurrentScene->getState().mina->resetMovement();

    if (IsKeyDown(KEY_A))
        gCurrentScene->getState().mina->moveLeft();
    else if (IsKeyDown(KEY_D))
        gCurrentScene->getState().mina->moveRight();

    if (IsKeyPressed(KEY_W) && gCurrentScene->getState().mina->isCollidingBottom()) {
        gCurrentScene->getState().mina->jump();
        PlaySound(gCurrentScene->getState().jumpSound);
    }

    if (GetLength(gCurrentScene->getState().mina->getMovement()) > 1.0f)
        gCurrentScene->getState().mina->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
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
        gCurrentScene->update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
    }
}

void render() {
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown() {
    delete gLevelA;

    for (int i = 0; i < NUMBER_OF_LEVELS; i++) gLevels[i] = nullptr;

    CloseAudioDevice();
    CloseWindow();
}

int main() {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0) {
            int id = gCurrentScene->getState().nextSceneID;
            // TODO: make this actually switch later
            // switchToScene(gLevels[id]);
            gAppStatus = TERMINATED;
            break;
        }

        render();
    }

    shutdown();

    return 0;
}
