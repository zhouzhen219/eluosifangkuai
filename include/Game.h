#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include "Tetris.h"

class Game
{
public:
    Game();
    ~Game();
    void gameRun();

private:
    enum class ResultPhase
    {
        Playing,
        EliminationNotice,
        VictoryNotice,
        Finished
    };

    void gameInitial();
    void LoadMediaData();
    void gameInput();
    void gameLogic();
    void gameDraw();
    void startResultSequence(int loserPlayer, int winnerPlayer);
    void drawResultOverlay();

    sf::RenderWindow window;
    Tetris player1, player2;
    bool isGameOver;
    bool isGameQuit;
    ResultPhase resultPhase;
    int resultLoserPlayer;
    int resultWinnerPlayer;
    sf::Clock resultClock;
    sf::Font uiFont;
    bool hasUiFont;

    sf::Texture tBackground, tTiles;
    sf::Texture emptyTexture;
    sf::Sprite sBackground;

    int window_width, window_height;
    int imgBGno, imgSkinNo;
    sf::Clock gameClock;
};