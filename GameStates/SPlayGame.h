#pragma once

#include "GameState.h"
#include "Map.h"
#include "Player.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "WTimer.h"
#include "UserInterface.h"
#include <vector>

//main gameplay state
class SPlayGame : public GameState
{
private:
    std::unique_ptr<Map> m_map{ std::make_unique<Map>() };
    std::shared_ptr<Player> m_player{ std::make_shared<Player>() };
    std::unique_ptr<GameObjectManager> m_objectManager{ std::make_unique<GameObjectManager>() };
    Camera m_camera{};
    WTimer m_stepTimer;
    WTimer m_updateTimer;
    UserInterface m_UI{ m_player };
    double m_timeAccumulator{ 0.0 };

    double m_FPSTime{ 0.0 };
    int m_FPSCount{ 0 };

    void ladderClimbUp();
    void ladderClimbDown();
    void jump();
    void holdLeftDodge();
    void holdRightDodge();
    void defaultDodge();

    void playerControlsHold(SDL_GameController* controller);
    void playerControlsPress(SDL_Event& event, SDL_GameController* controller);
    void meleeAttackController(SDL_GameController* controller, Sint16 currentLeftyState, Sint16 currentLeftxState);
    void rangedAttackController(SDL_GameController* controller, Sint16 currentLeftyState, Sint16 currentLeftxState);
    bool m_bumperControllerConfig{ true };

    double averageFPS(double timeStep);

    bool m_paused{ false };

    //sounds effects
    SoundEffect m_jumpSound{ "Assets/Sounds/PlayerSounds/jump.wav" };
    Mix_Music* m_music = Mix_LoadMUS("Assets/Sounds/Music/level1.wav");

public:
    SPlayGame(std::string mapFile = "Assets/Maps/sideScroller.txt");
    ~SPlayGame();

    State handleEvents(SDL_GameController* controller);
    State update();
    void render();

    void pauseGame() { m_paused = true; }
    void resumeGame() { m_paused = false; };
};

