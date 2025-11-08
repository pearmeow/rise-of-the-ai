#include "MainMenu.h"

#include <raylib.h>

#include "Entity.h"

MainMenu::MainMenu() : Scene{{0.0f}, nullptr} {
}

MainMenu::MainMenu(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

MainMenu::~MainMenu() {
    shutdown();
}

void MainMenu::initialise() {
    mGameState.nextSceneID = 0;
}

void MainMenu::update(float deltaTime) {
    mGameState.camera = {0};
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void MainMenu::render() {
    DrawText("Press ENTER to start", mOrigin.x - 200.0f, mOrigin.y, 30, WHITE);
    ClearBackground(BLACK);
}

void MainMenu::shutdown() {
}
