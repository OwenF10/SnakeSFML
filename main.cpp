#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

int cant = 0;
bool startScreen = true;
bool game = false;
bool winScreen = false;
bool saveScreen = false;


using namespace std;
using namespace sf;

int randomBetween(int min, int max)
{
    return (rand()%(max - min)) + min;
}

const char* intToConstChar(int x)
{
    stringstream ss;
    ss << x;
    string str = ss.str();
    return str.c_str();
}
bool collisionCheck(Sprite r1, Sprite r2)
{
    if(r1.getPosition().x> r2.getPosition().x+r2.getLocalBounds().width)
        return false;
    if(r1.getPosition().x+r1.getLocalBounds().width < r2.getPosition().x)
        return false;
    if(r1.getPosition().y > r2.getPosition().y+r2.getLocalBounds().height)
        return false;
    if(r1.getPosition().y+r1.getLocalBounds().height < r2.getPosition().y)
        return false;
    return true;
}
void countSnakeEat()
{
    Font ttf;
    ttf.loadFromFile("fonts/berlin_normal.ttf");
    Text text;
    text.setFont(ttf);
    text.setPosition(350, 4);
    text.setString(intToConstChar(cant));
    text.setColor(Color::White);
    text.setCharacterSize(50);
}
int main()
{
    srand(time(0));
    double startPosX = 385;
    double startPosY = 255;
    int posX = randomBetween(25, 715);
    int posY = randomBetween(93, 456);
    int dir = 3;
    int frames = 0;
    int velocity = 5;

    Font ttf;
    ttf.loadFromFile("fonts/berlin_normal.ttf");
    Text text;
//    text.setFont(ttf);
//    text.setPosition(350, 4);
//    text.setString(intToConstChar(cant));
//    text.setColor(Color::White);
//    text.setCharacterSize(50);

    // Create the main window
    RenderWindow app(VideoMode(797, 554, 32), "Snake");
    //Load Textures
    Texture snake;
    snake.loadFromFile("img/snake.png");
    Sprite snakeSprite(snake);
    snakeSprite.setPosition(startPosX, startPosY);

    Texture cuadrito;
    cuadrito.loadFromFile("img/cuadrito.png");
    Sprite cuadritoSprite(cuadrito);
    cuadritoSprite.setPosition(posX, posY);

    Texture start;
    start.loadFromFile("img/start.png");
    Sprite startSprite(start);

    Texture save;
    save.loadFromFile("img/saved.png");
    Sprite saveSprite(save);

    Texture background;
    background.loadFromFile("img/tablero.png");
    Sprite backgroundSprite(background);

    Texture gameover;
    gameover.loadFromFile("img/gameover.png");
    Sprite gameoverSprite(gameover);

    Texture win;
    win.loadFromFile("img/won.png");
    Sprite winSprite(win);

	// Start the game loop
    while (app.isOpen())
    {
        frames++;
        if(collisionCheck(snakeSprite, cuadritoSprite))
        {
            velocity++;
            posX = randomBetween(25, 715);
            posY = randomBetween(93, 456);
            cuadritoSprite.setPosition(posX, posY);
            cant++;
            cout << cant << endl;
            countSnakeEat();
        }

        // Process events
        Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if(event.type == Event::Closed)
                app.close();
            if(event.type == Event::KeyPressed)
            {
                if(event.key.code == Keyboard::Escape)
                    app.close();
                if(event.key.code == Keyboard::Left)
                    dir = 4;
                if(event.key.code == Keyboard::Right)
                    dir = 3;
                if(event.key.code == Keyboard::Down)
                    dir = 2;
                if(event.key.code == Keyboard::Up)
                    dir = 1;
                if(event.key.code == Keyboard::Space && startScreen)
                {
                    winScreen = false;
                    startScreen = false;
                    game = true;
                }
                if(event.key.code == Keyboard::Return && !game && !startScreen && !saveScreen)
                {
                    dir = 3;
                    game = true;
                    winScreen = false;
                    startScreen = false;
                    velocity = 5;
                    snakeSprite.setPosition(startPosX, startPosY);
                    posX = randomBetween(25, 715);
                    posY = randomBetween(93, 456);
                    cuadritoSprite.setPosition(posX, posY);
                }
                if(event.key.code == Keyboard::S && game)
                {
                    saveScreen = true;
                    ofstream out("SavedGames.txt");
                    out << snakeSprite.getPosition().x << " " << snakeSprite.getPosition().y << " " << velocity << " " << dir << " " << cant ;
                    out.close();
                    game = false;
                }
                if(event.key.code == Keyboard::L && startScreen)
                {
                    int x = 0;
                    int y = 0;
                    ifstream in("SavedGames.txt");
                    in >> x >> y >> velocity >> dir >> cant;
                    snakeSprite.setPosition(x,y);
                    in.close();
                    startScreen = false;
                    game = true;
                }
            }
        }
        if(frames == 90)
        {
            if(dir == 1)
            {
                snakeSprite.move(0, -velocity);
            }
            else if(dir == 2)
            {
                snakeSprite.move(0, velocity);
            }
            else if(dir == 3)
            {
                snakeSprite.move(velocity, 0);
            }
            else if(dir == 4)
            {
                snakeSprite.move(-velocity, 0);
            }
            frames = 0;
        }
        if(startScreen)
        {
            snakeSprite.setPosition(startPosX, startPosY);
            app.draw(startSprite);
        }
        if(winScreen)
        {
            snakeSprite.setPosition(startPosX, startPosY);
            app.draw(winSprite);
        }
        if((snakeSprite.getPosition().x <= 25 || snakeSprite.getPosition().x >= 715 ||
            snakeSprite.getPosition().y <= 93  || snakeSprite.getPosition().y >= 456) && game)
        {
            cant = 0;
            countSnakeEat();
            app.draw(gameoverSprite);
            game = false;
        }
        if(saveScreen)
        {
            app.draw(saveSprite);
            game = false;
            startScreen = false;
        }
        if(cant == 25)
        {
            cant = 0;
            winScreen = true;
            game = false;
            startScreen = false;
        }
        else if(game)
        {
            app.draw(backgroundSprite);
            app.draw(snakeSprite);
            app.draw(cuadritoSprite);
            app.draw(text);
        }
        app.display();
    }

    return EXIT_SUCCESS;
}
