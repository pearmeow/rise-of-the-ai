#include "WinScreen.h"

#include <raylib.h>

#include "Entity.h"

WinScreen::WinScreen() : Scene{{0.0f}, nullptr} {
}

WinScreen::WinScreen(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

WinScreen::~WinScreen() {
    shutdown();
}

void WinScreen::initialise() {
    mGameState.nextSceneID = 0;
}

void WinScreen::update(float deltaTime) {
    mGameState.camera = {0};
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void WinScreen::render() {
    DrawText("You win! Press enter to play again", mOrigin.x - 200.0f, mOrigin.y, 30, WHITE);
    ClearBackground(BLACK);
}

void WinScreen::shutdown() {
}
