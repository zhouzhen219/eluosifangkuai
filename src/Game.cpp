#include "Game.h"

Game::Game()
    : window(), player1(), player2(), isGameOver(false), isGameQuit(false), resultPhase(ResultPhase::Playing),
      resultLoserPlayer(0), resultWinnerPlayer(0), resultClock(), uiFont(), hasUiFont(false), tBackground(),
      tTiles(), emptyTexture(), sBackground(emptyTexture), window_width(1350), window_height(1000), imgBGno(1),
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

    hasUiFont = uiFont.openFromFile("C:/Windows/Fonts/msyh.ttc");
    if (!hasUiFont)
        hasUiFont = uiFont.openFromFile("C:/Windows/Fonts/simhei.ttf");
    if (!hasUiFont)
        hasUiFont = uiFont.openFromFile("C:/Windows/Fonts/arial.ttf");
}

void Game::gameInitial()
{
    window.setFramerateLimit(60);
    LoadMediaData();
    player1.Initial(&tTiles, &window, rolePLAYER1);
    player2.Initial(&tTiles, &window, rolePLAYER2);
    isGameOver = false;
    resultPhase = ResultPhase::Playing;
    resultLoserPlayer = 0;
    resultWinnerPlayer = 0;
    gameClock.restart();
    resultClock.restart();
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
        if (resultPhase == ResultPhase::Playing)
        {
            player1.Input(event);
            player2.Input(event);
        }
    }
}

void Game::startResultSequence(int loserPlayer, int winnerPlayer)
{
    resultLoserPlayer = loserPlayer;
    resultWinnerPlayer = winnerPlayer;
    resultPhase = ResultPhase::EliminationNotice;
    resultClock.restart();
}

void Game::gameLogic()
{
    if (resultPhase == ResultPhase::Playing)
    {
        float time = gameClock.restart().asSeconds();
        if (time > 0.033f)
            time = 0.033f;
        player1.timer += time;
        player2.timer += time;
        player1.Logic();
        player2.Logic();

        if (player1.isGameOver || player2.isGameOver)
        {
            if (player1.isGameOver && !player2.isGameOver)
            {
                startResultSequence(1, 2);
            }
            else if (player2.isGameOver && !player1.isGameOver)
            {
                startResultSequence(2, 1);
            }
            else
            {
                resultLoserPlayer = 0;
                resultWinnerPlayer = 0;
                resultPhase = ResultPhase::VictoryNotice;
                resultClock.restart();
                isGameOver = true;
            }
        }
        return;
    }

    float elapsed = resultClock.getElapsedTime().asSeconds();
    if (resultPhase == ResultPhase::EliminationNotice && elapsed >= 1.5f)
    {
        resultPhase = ResultPhase::VictoryNotice;
        resultClock.restart();
    }
    else if (resultPhase == ResultPhase::VictoryNotice && elapsed >= 1.5f)
    {
        isGameOver = true;
    }
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

    if (resultPhase != ResultPhase::Playing)
        drawResultOverlay();

    window.display();
}

void Game::drawResultOverlay()
{
    if (!hasUiFont)
        return;

    sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window_width), static_cast<float>(window_height)));
    overlay.setFillColor(sf::Color(0, 0, 0, 165));
    window.draw(overlay);

    sf::Text title(uiFont);
    title.setCharacterSize(54);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color::White);

    sf::Text detail(uiFont);
    detail.setCharacterSize(32);
    detail.setFillColor(sf::Color(230, 230, 230));

    if (resultPhase == ResultPhase::EliminationNotice)
    {
        title.setString(resultLoserPlayer == 1 ? "Player 1 eliminated" : "Player 2 eliminated");
        detail.setString("Bomb card triggered, player removed from the match");
    }
    else if (resultPhase == ResultPhase::VictoryNotice)
    {
        if (resultWinnerPlayer == 0)
        {
            title.setString("Draw");
            detail.setString("Both players were eliminated");
        }
        else
        {
            title.setString(resultWinnerPlayer == 1 ? "Player 1 wins" : "Player 2 wins");
            detail.setString("The other player has been eliminated");
        }
    }
    else
    {
        title.setString("Match over");
        detail.setString("No winner could be determined");
    }

    auto titleBounds = title.getLocalBounds();
    title.setOrigin(titleBounds.getCenter());
    title.setPosition(sf::Vector2f(static_cast<float>(window_width) * 0.5f, static_cast<float>(window_height) * 0.42f));

    auto detailBounds = detail.getLocalBounds();
    detail.setOrigin(detailBounds.getCenter());
    detail.setPosition(sf::Vector2f(static_cast<float>(window_width) * 0.5f, static_cast<float>(window_height) * 0.52f));

    window.draw(title);
    window.draw(detail);
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