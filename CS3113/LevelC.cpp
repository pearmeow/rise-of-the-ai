#include "LevelC.h"

#include <raylib.h>

#include "Entity.h"

LevelC::LevelC() : Scene{{0.0f}, nullptr} {
}
LevelC::LevelC(Vector2 origin, const char* bgHexCode) : Scene{origin, bgHexCode} {
}

LevelC::~LevelC() {
    shutdown();
}

void LevelC::initialise() {
    mGameState.nextSceneID = 0;

    if (!mGameState.mina) {
        mGameState.jumpSound = LoadSound("./assets/game/sfx_jump.ogg");
        SetSoundVolume(mGameState.jumpSound, 0.40f);
        mGameState.deathSound = LoadSound("./assets/game/death.wav");
        SetSoundVolume(mGameState.deathSound, 0.40f);
        mGameState.walkSound = LoadSound("./assets/game/walking-on-grass.wav");
        SetSoundVolume(mGameState.walkSound, 0.40f);
    }

    /*
       ----------- MAP -----------
    */
    if (!mGameState.map) {
        mGameState.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT,                      // map grid cols & rows
                                 (unsigned int*)mLevelData,                      // grid data
                                 "./assets/game/spritesheet-tiles-default.png",  // texture filepath
                                 TILE_DIMENSION,                                 // tile size
                                 18, 18,                                         // texture cols & rows
                                 {mOrigin.x, mOrigin.y}                          // in-game origin
        );
    }

    /*
       ----------- PROTAGONIST -----------
    */
    std::map<Direction, std::vector<int>> minaAnimationAtlas = {
        {DOWN, {12}},
        {LEFT, {35, 51}},
        {UP, {43}},
        {RIGHT, {35, 51}},
    };

    float sizeRatio = 48.0f / 64.0f;

    if (mGameState.mina) {
        mGameState.mina->resetMovement();
        mGameState.mina->activate();
        mGameState.mina->setPosition({mOrigin.x - 800.0f, mOrigin.y + 180.0f});
    } else {
        mGameState.mina = new Entity({mOrigin.x - 835.0f, mOrigin.y + 180.0f},            // position
                                     {75.0f, 50.0f},                                      // scale
                                     "./assets/game/spritesheet-characters-default.png",  // texture file address
                                     ATLAS,                                               // single image or atlas?
                                     {8, 8},                                              // atlas dimensions
                                     minaAnimationAtlas,                                  // actual atlas
                                     PLAYER                                               // entity type
        );

        mGameState.mina->setJumpingPower(750.0f);
        mGameState.mina->setColliderDimensions({mGameState.mina->getColliderDimensions().x * 2.0f / 3.0f,
                                                mGameState.mina->getColliderDimensions().y});
        mGameState.mina->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});
        mGameState.mina->setFrameSpeed(6);
    }

    std::map<Direction, std::vector<int>> skoudeAnimationAtlas = {
        {LEFT, {9, 17}},
        {DOWN, {9}},
        {RIGHT, {9, 17}},
        {UP, {9}},
    };

    if (mGameState.skoude) {
        mGameState.skoude->setAIState(IDLE);
        mGameState.skoude->activate();
        mGameState.skoude->setPosition(
            {mGameState.mina->getPosition().x - 200.0f, mGameState.mina->getPosition().y - 200.0f});
    } else {
        mGameState.skoude = new Entity(
            {mGameState.mina->getPosition().x - 200.0f, mGameState.mina->getPosition().y - 200.0f},  // position
            {100.0f, 100.0f},                                                                        // scale
            "./assets/game/spritesheet-enemies-default.png",  // texture file address
            ATLAS,                                            // single image or atlas?
            {8, 8},                                           // atlas dimensions
            skoudeAnimationAtlas,                             // actual atlas
            NPC                                               // entity type
        );

        mGameState.skoude->setAIType(GHOST);
        mGameState.skoude->setSpeed(50);
        mGameState.skoude->setAcceleration({0.0f, 0.0f});
        mGameState.skoude->setFrameSpeed(6);
    }

    /*
       ----------- CAMERA -----------
    */
    mGameState.camera = {0};                                    // zero initialize
    mGameState.camera.target = mGameState.mina->getPosition();  // camera follows player
    mGameState.camera.offset = mOrigin;                         // camera offset to center of screen
    mGameState.camera.rotation = 0.0f;                          // no rotation
    mGameState.camera.zoom = 1.0f;                              // default zoom
}

void LevelC::update(float deltaTime) {
    mGameState.skoude->update(deltaTime,  // delta time / fixed timestep
                              mGameState.mina,
                              nullptr,  // ghosts don't collide
                              nullptr,  // collidable entities
                              0         // col. entity count
    );

    mGameState.mina->update(deltaTime,          // delta time / fixed timestep
                            nullptr,            // player if entity is enemy
                            mGameState.map,     // map
                            mGameState.skoude,  // collidable entities
                            1                   // col. entity count
    );

    Vector2 currentPlayerPosition = {mGameState.mina->getPosition().x, mOrigin.y};

    if (mGameState.mina->getPosition().y > 800.0f) mGameState.nextSceneID = 4;

    panCamera(&mGameState.camera, &currentPlayerPosition);
}

void LevelC::render() {
    ClearBackground(ColorFromHex(mBGColourHexCode));

    mGameState.mina->render();
    mGameState.map->render();
    mGameState.skoude->render();
}

void LevelC::shutdown() {
    delete mGameState.mina;
    delete mGameState.skoude;
    delete mGameState.map;

    UnloadSound(mGameState.jumpSound);
}
