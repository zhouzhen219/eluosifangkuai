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
    void gameInitial();
    void LoadMediaData();
    void gameInput();
    void gameLogic();
    void gameDraw();

    sf::RenderWindow window;
    Tetris player1, player2;
    bool isGameOver;
    bool isGameQuit;

    sf::Texture tBackground, tTiles;
    sf::Texture emptyTexture;
    sf::Sprite sBackground;

    int window_width, window_height;
    int imgBGno, imgSkinNo;
    sf::Clock gameClock;
};