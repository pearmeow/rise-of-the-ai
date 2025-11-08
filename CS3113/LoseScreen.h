#include "LevelA.h"

#ifndef LOSE_SCREEN_H
#define LOSE_SCREEN_H

class LoseScreen : public Scene {
public:
    LoseScreen();
    LoseScreen(Vector2 origin, const char* bgHexCode);
    ~LoseScreen();

    void initialise() override;
    void update(float deltaTime) override;
    void render() override;
    void shutdown() override;
};

#endif
