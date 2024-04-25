#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <pthread.h>
#include <sys/wait.h>
using namespace std;
using namespace sf;

struct ThreadArgs
{
    int f_num;
    RenderWindow *window;
};

void *threadFunction(void *arg)
{
    ThreadArgs *args = static_cast<ThreadArgs *>(arg);

    while (args->window->isOpen())
    {
        // update the f_num value
        args->f_num++;
        sleep(1);
    }

    return NULL;
}

const int num_coins = 5;

int main()
{
    const int COUNTDOWN_DURATION = 60; // In seconds

    pthread_t t1;
    // step 1
    int f_num = 0;
    srand((unsigned)time(NULL));
    int r_num = 10 + (rand() % 101);
    cout << "Random number :" << r_num << endl;
    // step 2
    int m_num = 0;
    m_num = r_num * 2;
    cout << "After multiplication :" << m_num << endl;
    // step 3
    int ro_num = 542 / m_num;
    cout << "Roll number after division :" << ro_num << endl;
    // step 4
    f_num = ro_num % 25;
    cout << "Number we actually get :" << f_num << endl;
    if (f_num < 15)
    {
        f_num = f_num + 20;
    }
    cout << "Final Number :" << f_num << endl;
    // f_num = 10;
    int box_siz = 50;
    RenderWindow window(sf::VideoMode(820, 500), "Game");
    // create a rectangle shape for each cell in the grid
    RectangleShape box[f_num][f_num];
    for (int i = 0; i < f_num; i++)
    {
        for (int j = 0; j < f_num; j++)
        {
            box[i][j].setSize(Vector2f(box_siz, box_siz));
            box[i][j].setPosition(j * box_siz, i * box_siz);
            box[i][j].setFillColor(Color::White);
            box[i][j].setOutlineThickness(1);
            box[i][j].setOutlineColor(Color::Red);
        }
    }

    /// Music
    sf::Music music;
    if (!music.openFromFile("m.wav"))
    {
        std::cout << "Error loading music file" << std::endl;
        return -1;
    }
    music.play();
    music.setVolume(50); // set the volume to 50%
    music.setLoop(true); // loop the music

    //----------------------
    Font font;
    font.loadFromFile("Eastwood.ttf");
    if (!font.loadFromFile("Eastwood.ttf"))
    {
        cerr << "Failed to load font file." << endl;
        // return -1;
    }

    int score1 = 0;
    int score2 = 0;

    Text scoreText1, scoreText2;
    scoreText1.setFont(font);
    scoreText1.setString("Player 1 Score: " + to_string(score1));
    scoreText1.setCharacterSize(25);
    scoreText1.setFillColor(sf::Color::Red);
    scoreText1.setPosition(520, 35);

    scoreText2.setFont(font);
    scoreText2.setString("Player 2 Score: " + to_string(score2));
    scoreText2.setCharacterSize(25);
    scoreText2.setFillColor(sf::Color::Red);
    scoreText2.setPosition(520, 65);

    Text t, p1;
    t.setFont(font); // font is a sf::Font
    t.setString("Hello world");
    t.setCharacterSize(25); // in pixels, not points!
    t.setFillColor(sf::Color::Red);
    t.setPosition(520, 5); // set axis of hello world
    t.setStyle(Text::Bold | Text::Underlined);
    //--------------------
    float vx = 0.f;
    float vy = 0.f;
    //--------------------
    Texture img;
    int x = 50, y = 50;
    if (!img.loadFromFile("Player1.png"))
    {
        return 1;
    }
    Sprite sp_img1(img);
    sp_img1.setPosition(x, y);
    Texture img2;
    if (!img2.loadFromFile("Player2.jpeg"))
    {
        return 1;
    }
    Sprite sp_img2(img2);
    sp_img2.setPosition(300, 50);

    // Change this block of code to create an array of coins
    Texture coin;
    if (!coin.loadFromFile("coin.png"))
    {
        return 1;
    }
    Sprite s_coin[num_coins];
    for (int i = 0; i < num_coins; i++)
    {
        s_coin[i] = Sprite(coin);
        s_coin[i].setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
    }

    ThreadArgs args = {f_num, &window};

    // create a new thread and pass the arguments
    pthread_t threadId;
    pthread_create(&threadId, NULL, threadFunction, &args);

    sf::Clock clock;
    int countdown = COUNTDOWN_DURATION;

    Text countdownText;
    countdownText.setFont(font);
    countdownText.setString("Time Left: " + to_string(countdown));
    countdownText.setCharacterSize(25);
    countdownText.setFillColor(sf::Color::Red);
    countdownText.setPosition(520, 95);

    //--------------------------
    while (window.isOpen())
    {
        float elapsedTime = clock.getElapsedTime().asSeconds();
        countdown = COUNTDOWN_DURATION - static_cast<int>(elapsedTime);
        countdownText.setString("Time Left: " + to_string(countdown));

        Event oe;
        while (window.pollEvent(oe))
        {
            if (oe.type == Event::Closed)
            { //
                window.close();
            }
        }
        //-----------------for red one moving code
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            sp_img1.move(-0.1, 0.0);
            // sp_img1.rotate(180);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            sp_img1.move(0.1, 0.0);
            // sp_img1.rotate(180);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            sp_img1.move(0.0, -0.1);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            sp_img1.move(0.0, 0.1);
        }
        // to keep 1st one in boundary
        FloatRect b_img1 = sp_img1.getGlobalBounds();
        if (b_img1.left < 0.0f)
        {
            sp_img1.setPosition(0.0f, sp_img1.getPosition().y);
        }
        else if (b_img1.left + b_img1.width > window.getSize().x)
        {
            sp_img1.setPosition(window.getSize().x - b_img1.width, sp_img1.getPosition().y);
        }
        if (b_img1.top < 0.0f)
        {
            sp_img1.setPosition(sp_img1.getPosition().x, 0.0f);
        }
        else if (b_img1.top + b_img1.height > window.getSize().y)
        {
            sp_img1.setPosition(sp_img1.getPosition().x, window.getSize().y - b_img1.height);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            exit(1);
        }
        //----------------for moving other one
        if (Keyboard::isKeyPressed(Keyboard::A))
        {

            sp_img2.move(-0.1, 0.0);
        }
        else if (Keyboard::isKeyPressed(Keyboard::D))
        {
            sp_img2.move(0.1, 0.0);
        }
        else if (Keyboard::isKeyPressed(Keyboard::S))
        {
            sp_img2.move(0.0, -0.1);
        }
        else if (Keyboard::isKeyPressed(Keyboard::W))
        {
            sp_img2.move(0.0, 0.1);
        }
        // apply boundary conditions on second one
        FloatRect b_img2 = sp_img2.getGlobalBounds();
        if (b_img2.left < 0.0f)
        {
            sp_img2.setPosition(0.0f, sp_img2.getPosition().y);
        }
        else if (b_img2.left + b_img2.width > window.getSize().x)
        {
            sp_img2.setPosition(window.getSize().x - b_img2.width, sp_img2.getPosition().y);
        }
        if (b_img2.top < 0.0f)
        {
            sp_img2.setPosition(sp_img2.getPosition().x, 0.0f);
        }
        else if (b_img2.top + b_img2.height > window.getSize().y)
        {
            sp_img2.setPosition(sp_img2.getPosition().x, window.getSize().y - b_img2.height);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            exit(1);
        }

        // Check for collisions with coins and remove them if necessary
        for (int i = 0; i < num_coins; i++)
        {
            if (sp_img1.getGlobalBounds().intersects(s_coin[i].getGlobalBounds()))
            {
                s_coin[i].setPosition(rand() % 820, rand() % 500);            // Move the coin off-screen
                score1++;                                                     // Increase Player 1's score
                scoreText1.setString("Player 1 Score: " + to_string(score1)); // Update the displayed score
            }
            if (sp_img2.getGlobalBounds().intersects(s_coin[i].getGlobalBounds()))
            {
                s_coin[i].setPosition(rand() % 820, rand() % 500);            // Move the coin off-screen
                score2++;                                                     // Increase Player 2's score
                scoreText2.setString("Player 2 Score: " + to_string(score2)); // Update the displayed score
            }
        }
        if (countdown <= 0)
        {
            window.close();
        }

        window.clear(Color::Black);
        for (int i = 0; i < f_num; i++)
        {
            for (int j = 0; j < f_num; j++)
            {
                window.draw(box[i][j]);
            }
        }
        window.draw(sp_img1);
        window.draw(sp_img2);
        window.draw(scoreText1);
        window.draw(scoreText2);
        window.draw(countdownText);

        // Draw the coins
        for (int i = 0; i < num_coins; i++)
        {
            window.draw(s_coin[i]);
        }

        window.draw(t);
        window.display();
    }

    // join the thread
    pthread_join(threadId, NULL);

    return 0;
}
