#include "LevelA.h"

LevelA::LevelA() : Scene{{0.0f}, nullptr} {
}
LevelA::LevelA(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelA::~LevelA() {
    shutdown();
}

void LevelA::initialise() {
    mGameState.nextSceneID = 0;

    // TODO: get soundtrack
    mGameState.bgm = LoadMusicStream("");
    SetMusicVolume(mGameState.bgm, 0.33f);
    // PlayMusicStream(gState.bgm);

    // TODO: get jump sound
    mGameState.jumpSound = LoadSound("");

    /*
       ----------- MAP -----------
    */
    mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,  // map grid cols & rows
                             (unsigned int*)mLevelData,  // grid data
                             // TODO: get atlas for tileset
                             "",              // texture filepath
                             TILE_DIMENSION,  // tile size
                             // TODO: change dimensions of cols & rows
                             4, 1,    // texture cols & rows
                             mOrigin  // in-game origin
    );

    /*
       ----------- PROTAGONIST -----------
    */
    // TODO: change coords in vectors to match atlas
    std::map<Direction, std::vector<int>> minaAnimationAtlas = {
        {DOWN, {0, 1, 2, 3, 4, 5, 6, 7}},
        {LEFT, {8, 9, 10, 11, 12, 13, 14, 15}},
        {UP, {24, 25, 26, 27, 28, 29, 30, 31}},
        {RIGHT, {40, 41, 42, 43, 44, 45, 46, 47}},
    };

    float sizeRatio = 48.0f / 64.0f;

    // TODO: rename this variable
    // Assets from @see https://sscary.itch.io/the-adventurer-female
    mGameState.mina = new Entity({mOrigin.x - 300.0f, mOrigin.y - 200.0f},  // position
                                 {250.0f * sizeRatio, 250.0f},              // scale
                                 // TODO: get new atlas
                                 "",                  // texture file address
                                 ATLAS,               // single image or atlas?
                                 {6, 8},              // atlas dimensions
                                 minaAnimationAtlas,  // actual atlas
                                 PLAYER               // entity type
    );

    mGameState.mina->setJumpingPower(550.0f);
    mGameState.mina->setColliderDimensions(
        {mGameState.mina->getScale().x / 3.5f, mGameState.mina->getScale().y / 3.0f});
    mGameState.mina->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    /*
       ----------- CAMERA -----------
    */
    // TODO: make the camera not follow the player but scroll and also make player die when offscreen
    mGameState.camera = {0};                                    // zero initialize
    mGameState.camera.target = mGameState.mina->getPosition();  // camera follows player
    mGameState.camera.offset = mOrigin;                         // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;                          // no rotation
    mGameState.camera.zoom = 1.0f;                              // default zoom
}

void LevelA::update(float deltaTime) {
    UpdateMusicStream(mGameState.bgm);

    mGameState.mina->update(deltaTime,       // delta time / fixed timestep
                            nullptr,         // player
                            mGameState.map,  // map
                            nullptr,         // collidable entities
                            0                // col. entity count
    );

    Vector2 currentPlayerPosition = {mGameState.mina->getPosition().x, mOrigin.y};

    if (mGameState.mina->getPosition().y > 800.0f) mGameState.nextSceneID = 1;

    panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelA::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.mina->render();
    mGameState.map->render();
}

void LevelA::shutdown() {
    delete mGameState.mina;
    delete mGameState.map;

    UnloadMusicStream(mGameState.bgm);
    UnloadSound(mGameState.jumpSound);
}
