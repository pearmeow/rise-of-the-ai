#include "Scene.h"

constexpr int LEVEL_WIDTH = 25, LEVEL_HEIGHT = 8;

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
