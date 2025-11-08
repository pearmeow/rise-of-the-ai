#include "LevelA.h"

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

class MainMenu : public Scene {
public:
    MainMenu();
    MainMenu(Vector2 origin, const char* bgHexCode);
    ~MainMenu();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
