#include "LevelA.h"

#ifndef WIN_SCREEN_H
#define WIN_SCREEN_H

class WinScreen : public Scene {
public:
    WinScreen();
    WinScreen(Vector2 origin, const char* bgHexCode);
    ~WinScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
