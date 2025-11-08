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
}

void MainMenu::update(float deltaTime) {
}

void MainMenu::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));
    // render some text here
}

void MainMenu::shutdown() {
    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
}
