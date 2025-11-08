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

#include <raylib.h>

#include <charconv>
#include <cstdlib>

#include "CS3113/LevelA.h"
#include "CS3113/LevelB.h"
#include "CS3113/LevelC.h"
#include "CS3113/LoseScreen.h"
#include "CS3113/MainMenu.h"
#include "CS3113/WinScreen.h"
#include "CS3113/cs3113.h"

// Global Constants
constexpr int SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 600, FPS = 120, NUMBER_OF_LEVELS = 6;

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr float FIXED_TIMESTEP = 1.0f / 60.0f;

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f, gTimeAccumulator = 0.0f;
Music gBgm;

Scene* gCurrentScene = nullptr;
int gLives = 3;
char gLivesText[20] = "Lives: ";
Vector2 gLivesTextPos = {0.0f, 0.0f};
int gSceneIndex = 0;
std::vector<Scene*> gLevels = {};

bool gPlayWalkSound = false;
int gTimeframes = 0;
MainMenu* gMainMenu = nullptr;
LevelA* gLevelA = nullptr;
LevelB* gLevelB = nullptr;
LevelC* gLevelC = nullptr;
WinScreen* gWinScreen = nullptr;
LoseScreen* gLoseScreen = nullptr;

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
    gBgm = LoadMusicStream("./assets/game/Mesmerizing Galaxy Loop.mp3");
    SetMusicVolume(gBgm, 0.10f);
    PlayMusicStream(gBgm);

    gMainMenu = new MainMenu(ORIGIN, "#FFFFFF");
    gLevelA = new LevelA(ORIGIN, "#C0897E");
    gLevelB = new LevelB(ORIGIN, "#AAAAAA");
    gLevelC = new LevelC(ORIGIN, "#EEEEEE");
    gWinScreen = new WinScreen(ORIGIN, "#FFFFFF");
    gLoseScreen = new LoseScreen(ORIGIN, "#FFFFFF");

    gLevels.push_back(gMainMenu);
    gLevels.push_back(gLevelA);
    gLevels.push_back(gLevelB);
    gLevels.push_back(gLevelC);
    gLevels.push_back(gWinScreen);
    gLevels.push_back(gLoseScreen);

    switchToScene(gLevels[0]);

    SetTargetFPS(FPS);
}

void processInput() {
    if (gCurrentScene->getState().mina) {
        gCurrentScene->getState().mina->resetMovement();

        if (IsKeyDown(KEY_A)) {
            gCurrentScene->getState().mina->moveLeft();
            if (abs(gCurrentScene->getState().mina->getVelocity().y) <= 0.001) {
                gPlayWalkSound = true;
            }
        } else if (IsKeyDown(KEY_D)) {
            gCurrentScene->getState().mina->moveRight();
            if (abs(gCurrentScene->getState().mina->getVelocity().y) <= 0.001) {
                gPlayWalkSound = true;
            }
        }

        if (IsKeyPressed(KEY_W) && gCurrentScene->getState().mina->isCollidingBottom()) {
            gCurrentScene->getState().mina->jump();
            PlaySound(gCurrentScene->getState().jumpSound);
        }

        if (GetLength(gCurrentScene->getState().mina->getMovement()) > 1.0f)
            gCurrentScene->getState().mina->normaliseMovement();
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (gSceneIndex == NUMBER_OF_LEVELS - 1 || gSceneIndex == NUMBER_OF_LEVELS - 2) {
            gLives = 3;
            gSceneIndex = 0;
            switchToScene(gLevels[gSceneIndex]);
        } else if (gSceneIndex == 0) {
            gLives = 3;
            gSceneIndex = 1;
            switchToScene(gLevels[gSceneIndex]);
        }
    }

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() {
    UpdateMusicStream(gBgm);

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
        if (gPlayWalkSound && gTimeframes == 0) {
            PlaySound(gCurrentScene->getState().walkSound);
            gPlayWalkSound = false;
            ++gTimeframes;
        } else if (gPlayWalkSound) {
            ++gTimeframes;
            if (gTimeframes == 20) {
                gTimeframes = 0;
            }
        }
    }
    if (gCurrentScene->getState().mina) {
        if (!gCurrentScene->getState().mina->isActive()) {
            PlaySound(gCurrentScene->getState().deathSound);
            --gLives;
            gCurrentScene->initialise();
        }
    }
    if (gLives <= 0) {
        gSceneIndex = NUMBER_OF_LEVELS - 1;
        switchToScene(gLevels[gSceneIndex]);
        gLives = 3;
    }
    std::to_chars(gLivesText + 6, gLivesText + 20, gLives);
}

void render() {
    BeginDrawing();
    BeginMode2D(gCurrentScene->getState().camera);

    DrawText(gLivesText, 10.0f, 10.0f, 30, BLACK);
    gCurrentScene->render();

    EndMode2D();
    EndDrawing();
}

void shutdown() {
    delete gLevelA;
    delete gMainMenu;
    delete gLoseScreen;
    delete gWinScreen;

    for (size_t i = 0; i < gLevels.size(); ++i) {
        gLevels[i] = nullptr;
    }

    UnloadMusicStream(gBgm);
    CloseAudioDevice();
    CloseWindow();
}

int main() {
    initialise();

    while (gAppStatus == RUNNING) {
        processInput();
        update();

        if (gCurrentScene->getState().nextSceneID > 0) {
            gSceneIndex = gCurrentScene->getState().nextSceneID;
            switchToScene(gLevels[gSceneIndex]);
        }

        render();
    }

    shutdown();

    return 0;
}
