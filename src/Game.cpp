#include "Game.h"

Game::Game()
    : window(), player1(), player2(), isGameOver(false), isGameQuit(false), tBackground(), tTiles(),
      emptyTexture(), sBackground(emptyTexture), window_width(1350), window_height(1000), imgBGno(1),
      imgSkinNo(1), gameClock()
{
    window.create(sf::VideoMode({static_cast<unsigned int>(window_width), static_cast<unsigned int>(window_height)}), "Dual Tetris");
    window.setKeyRepeatEnabled(false);
    gameInitial();
}

Game::~Game() {}

void Game::LoadMediaData()
{
    // 尝试加载背景纹理（失败则生成白色）
    std::stringstream ss;
    ss << "../data/images/bg" << imgBGno << ".jpg";
    if (!tBackground.loadFromFile(ss.str()))
    {
        sf::Image bgImage({1u, 1u}, sf::Color::White);
        tBackground = sf::Texture(bgImage);
    }
    // 尝试加载方块纹理，失败则动态生成7种颜色的纹理
    ss.str("");
    ss << "../data/images/tiles" << imgSkinNo << ".jpg";
    if (!tTiles.loadFromFile(ss.str()))
    {
        sf::Image img({static_cast<unsigned int>(GRIDSIZE * 8), static_cast<unsigned int>(GRIDSIZE)},
                      sf::Color::Black);
        for (int i = 1; i <= 7; ++i)
        {
            sf::Color col;
            switch (i)
            {
            case 1:
                col = sf::Color::Cyan;
                break;
            case 2:
                col = sf::Color::Yellow;
                break;
            case 3:
                col = sf::Color::Magenta;
                break;
            case 4:
                col = sf::Color::Green;
                break;
            case 5:
                col = sf::Color::Red;
                break;
            case 6:
                col = sf::Color::Blue;
                break;
            case 7:
                col = sf::Color::White;
                break;
            }
            for (unsigned int y = 0; y < GRIDSIZE; ++y)
                for (unsigned int x = 0; x < GRIDSIZE; ++x)
                    img.setPixel({static_cast<unsigned int>(i * GRIDSIZE + x), y}, col);
        }
        tTiles = sf::Texture(img);
    }
    sBackground.setTexture(tBackground, true);
}

void Game::gameInitial()
{
    window.setFramerateLimit(60);
    LoadMediaData();
    player1.Initial(&tTiles, &window, rolePLAYER1);
    player2.Initial(&tTiles, &window, rolePLAYER2);
    isGameOver = false;
    gameClock.restart();
}

void Game::gameInput()
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
            isGameQuit = true;
        }
        player1.Input(event);
        player2.Input(event);
    }
}

void Game::gameLogic()
{
    float time = gameClock.restart().asSeconds();
    if (time > 0.033f)
        time = 0.033f;
    player1.timer += time;
    player2.timer += time;
    player1.Logic();
    player2.Logic();
    // 原逻辑：任一玩家死亡即结束
    // 修改为：仅当双方都死亡时才结束，另一方可以继续游戏
    isGameOver = (player1.isGameOver && player2.isGameOver);
}

void Game::gameDraw()
{
    window.clear(sf::Color::Black);
    window.draw(sBackground);
    // 绘制玩家边框，帮助定位舞台边界
    {
        sf::RectangleShape rect1(sf::Vector2f(static_cast<float>(STAGE_WIDTH * GRIDSIZE), static_cast<float>(STAGE_HEIGHT * GRIDSIZE)));
        rect1.setPosition(sf::Vector2f(static_cast<float>(player1.getCorner().x), static_cast<float>(player1.getCorner().y)));
        rect1.setFillColor(sf::Color::Transparent);
        rect1.setOutlineColor(sf::Color::White);
        rect1.setOutlineThickness(2.0f);
        window.draw(rect1);

        sf::RectangleShape rect2(rect1);
        rect2.setPosition(sf::Vector2f(static_cast<float>(player2.getCorner().x), static_cast<float>(player2.getCorner().y)));
        rect2.setOutlineColor(sf::Color::Cyan);
        window.draw(rect2);
    }
    player1.Draw();
    player2.Draw();
    window.display();
}

void Game::gameRun()
{
    while (window.isOpen() && !isGameQuit && !isGameOver)
    {
        gameInput();
        gameLogic();
        gameDraw();
    }
    // 游戏结束后等待2秒自动关闭
    sf::sleep(sf::seconds(2));
}