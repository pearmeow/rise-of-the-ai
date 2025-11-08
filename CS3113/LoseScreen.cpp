#include "LoseScreen.h"

#include <raylib.h>

#include "Entity.h"

LoseScreen::LoseScreen() : Scene{{0.0f}, nullptr} {
}

LoseScreen::LoseScreen(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LoseScreen::~LoseScreen() {
    shutdown();
}

void LoseScreen::initialise() {
    mGameState.nextSceneID = 0;
}

void LoseScreen::update(float deltaTime) {
    mGameState.camera = {0};
    mGameState.camera.target = mOrigin;
    mGameState.camera.offset = mOrigin;
    mGameState.camera.rotation = 0.0f;
    mGameState.camera.zoom = 1.0f;
}

void LoseScreen::render() {
    DrawText("You lose! Press enter to try again", mOrigin.x - 200.0f, mOrigin.y, 30, WHITE);
    ClearBackground(BLACK);
}

void LoseScreen::shutdown() {
}
