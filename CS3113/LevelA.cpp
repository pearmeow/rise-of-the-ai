#include "LevelA.h"

#include <raylib.h>

#include "Entity.h"

LevelA::LevelA() : Scene{{0.0f}, nullptr} {
}
LevelA::LevelA(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelA::~LevelA() {
    shutdown();
}

void LevelA::initialise() {
    mGameState.nextSceneID = 0;

    mGameState.bgm = LoadMusicStream("./assets/game/Mesmerizing Galaxy Loop.mp3");
    SetMusicVolume(mGameState.bgm, 0.50f);
    PlayMusicStream(mGameState.bgm);

    mGameState.jumpSound = LoadSound("./assets/game/sfx_jump.ogg");

    /*
       ----------- MAP -----------
    */
    mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,                      // map grid cols & rows
                             (unsigned int*)mLevelData,                      // grid data
                             "./assets/game/spritesheet-tiles-default.png",  // texture filepath
                             TILE_DIMENSION,                                 // tile size
                             18, 18,                                         // texture cols & rows
                             {mOrigin.x, mOrigin.y}                          // in-game origin
    );

    /*
       ----------- PROTAGONIST -----------
    */
    // TODO: change coords in vectors to match atlas
    std::map<Direction, std::vector<int>> minaAnimationAtlas = {
        {DOWN, {12}},
        {LEFT, {35, 51}},
        {UP, {43}},
        {RIGHT, {35, 51}},
    };

    float sizeRatio = 48.0f / 64.0f;

    mGameState.mina = new Entity({mOrigin.x - 300.0f, mOrigin.y - 200.0f},            // position
                                 {100.0f, 100.0f},                                    // scale
                                 "./assets/game/spritesheet-characters-default.png",  // texture file address
                                 ATLAS,                                               // single image or atlas?
                                 {8, 8},                                              // atlas dimensions
                                 minaAnimationAtlas,                                  // actual atlas
                                 PLAYER                                               // entity type
    );

    // TODO: make skoude
    // mGameState.skoude = new Entity();

    mGameState.mina->setJumpingPower(550.0f);
    mGameState.mina->setColliderDimensions(
        {mGameState.mina->getColliderDimensions().x * 2.0f / 3.0f, mGameState.mina->getColliderDimensions().y});
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

    mGameState.mina->update(deltaTime,          // delta time / fixed timestep
                            nullptr,            // player
                            mGameState.map,     // map
                            mGameState.skoude,  // collidable entities
                            0                   // col. entity count
    );

    // TODO: make skoude move

    Vector2 currentPlayerPosition = {mGameState.mina->getPosition().x, mOrigin.y};

    // TODO: make the player (probably touch something) to move to next level
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
