#include <iostream>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBee(float&, Sprite&, bool&, const Time&);
void updateCloud(std::vector<Sprite>&, std::vector<bool>&, std::vector<float>&, const Time&);
void updateBranches(int);

const int NUM_BRANCHES = 6;
const int NUM_CLOUDS = 3;

// Where is the player
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
    // Object for player resolution
    // Takes in an unsigned 2D int vector
    VideoMode vm({ 1920u, 1080u });
    //Object for rendering
    RenderWindow window(vm, "Timber!!!", State::Fullscreen);


    // Create texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("graphics/background.png")) {
        std::cout << "Couldn't load file!";
    }
    // Create a sprite using texture
    Sprite backgroundSprite(backgroundTexture);
    // Has to be a vector
    backgroundSprite.setPosition({ 0, 0 });

    const float TREE_HORIZONTAL_POSITION = 810;
    const float TREE_VERTICAL_POSITION = 0;

    // Tree sprite
    Texture treeTexture;
    if (!treeTexture.loadFromFile("graphics/tree.png")) {
        std::cout << "Couldn't load file!";
    }
    Sprite treeSprite(treeTexture);
    treeSprite.setPosition(Vector2f(TREE_HORIZONTAL_POSITION, TREE_VERTICAL_POSITION));

    float BEE_HORIZONTAL_POSITION = 0;
    float BEE_VERTICAL_POSITION = 800;

    // Bee Sprite
    Texture beeTexture;
    if (!beeTexture.loadFromFile("graphics/bee.png")) {
        std::cout << "Couldn't load file!";
    }
    Sprite beeSprite(beeTexture);
    beeSprite.setPosition({ BEE_HORIZONTAL_POSITION , BEE_VERTICAL_POSITION });

    // Flag to check if bee is moving
    bool beeActive = false;
    float beeSpeed = 0.0f;

    Texture cloudTexture;
    if (!cloudTexture.loadFromFile("graphics/cloud.png")) {
        std::cout << "Couldn't load file!";
    }

    std::vector <Sprite> clouds;
    clouds.reserve(NUM_CLOUDS);
    std::vector<bool> cloudsActive(NUM_CLOUDS, false);
    std::vector<float> cloudsSpeed(NUM_CLOUDS, 0.0f);

    for (int i = 0; i < NUM_CLOUDS; i++) 
    {
        clouds.emplace_back(cloudTexture);
        clouds[i].setPosition(Vector2f(-300, i * 150));
    }

    Clock clock;

    // Time bar for player time
    RectangleShape timeBar;

    // Helps keep track of what should be drawn each frame
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;

    //
    timeBar.setSize({ timeBarStartWidth, timeBarHeight });
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition({ (1920 / 2) - timeBarStartWidth, 980 });

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    bool gamePaused = true;

    int playerScore = 0;

    // Font
    Font textFont;
    if (!textFont.openFromFile("fonts/KOMIKAP_.ttf")) {
        std::cout << "Unable to load font file!!!";
    }

    // Pause screen text
    Text startingScreenText(textFont, "Press enter to start!");
    Text scoreText(textFont, "Score = 0");

    // Text Length
    startingScreenText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // Text Color
    startingScreenText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    // Creating text box
    FloatRect textRect{ startingScreenText.getLocalBounds() };

    // Measurmentnts of text box
    float width = textRect.size.x;
    float height = textRect.size.y;
    float left = textRect.position.x;
    float top = textRect.position.y;

    // Position of text
    startingScreenText.setOrigin({ left + width / 2.0f, top + height / 2.0f });
    startingScreenText.setPosition({ 1920 / 2.0f, 1080 / 2.0f });

    scoreText.setPosition({ 20, 20 });

    Texture branchTexture;
    if (!branchTexture.loadFromFile("graphics/branch.png")) {
        std::cout << "Couldn't load file!";
    }

    std::vector <Sprite> branches;
    branches.reserve(NUM_BRANCHES);

    for (int i = 0; i < NUM_BRANCHES; i++) 
    {
        branches.emplace_back(branchTexture);
        branches[i].setPosition(Vector2f(-2000.0f, -2000.0f));
        branches[i].setOrigin(Vector2f(220.0f, 20.0f));
    }

    // Prepare the player
    Texture playerTexture;
    if (!playerTexture.loadFromFile("graphics/player.png")) {
        std::cout << "Couldn't load file!";
    }
    Sprite playerSprite(playerTexture);
    playerSprite.setPosition({ 580, 720 });

    side playerSide{ side::LEFT };

    // Prepare the tombstone
    Texture ripTexture;
    if (!ripTexture.loadFromFile("graphics/rip.png")) {
        std::cout << "Couldn't load file!";
    }
    Sprite ripSprite(ripTexture);
    ripSprite.setPosition({ 600,860 });

    // Prepare the axe
    Texture axeTexture;
    if (!axeTexture.loadFromFile("graphics/axe.png")) {
        std::cout << "Couldn't load file!";
    }

    Sprite axeSprite(axeTexture);
    axeSprite.setPosition({ 700,830 });

    // Line the axe with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture logTexture;
    if (!logTexture.loadFromFile("graphics/log.png")) {
        std::cout << "Couldn't load file!";
    }

    Sprite logSprite(logTexture);
    logSprite.setPosition({ 810,720 });

    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // Control player input
    bool acceptInput = false;

    // Prepare sound
    SoundBuffer chopBuffer;
    if (!chopBuffer.loadFromFile("sound/chop.wav")) {
        std::cout << "Couldn't load file!";
    }
    Sound chop{ chopBuffer };
    chop.setVolume(50.0f);

    SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("sound/death.wav")) {
        std::cout << "Couldn't load file!";
    }
    Sound death{ deathBuffer };

    SoundBuffer ootBuffer;
    if (!ootBuffer.loadFromFile("sound/out_of_time.wav")) {
        std::cout << "Couldn't load file!";
    }
    Sound oot{ ootBuffer };

    // Game loop
    while (window.isOpen()) 
    {

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::KeyReleased>() && !gamePaused)
            {
                acceptInput = true;
                axeSprite.setPosition(Vector2f(2000.0f, axeSprite.getPosition().y));
            }
                
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))  // note Key::Escape
        {
            window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter)) 
        {
            gamePaused = false;

            playerScore = 0;
            timeRemaining = 6;

            // Removes branches temporarily
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }

            // Moves tombstone out of frame
            ripSprite.setPosition(Vector2f(675.0f, 2000.0f));
            // Moves player to position
            playerSprite.setPosition(Vector2f(580.0f, 720.0f));
            acceptInput = true;
        }

        if (acceptInput) 
        {

            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                // Move player and add to score
                playerSide = side::RIGHT;
                playerScore++;

                // Add time
                timeRemaining += (2 / playerScore) + 0.15;

                axeSprite.setPosition(Vector2f(AXE_POSITION_RIGHT, axeSprite.getPosition().y));
                playerSprite.setPosition(Vector2f(1200.0f, 720.0f));

                updateBranches(playerScore);

                logSprite.setPosition(Vector2f(810.0f, 720.0f));
                
                logSpeedX = -5000;
                logActive = true;
                acceptInput = false;

                chop.play();
            }
            if(Keyboard::isKeyPressed(Keyboard::Key::Left)) 
            {

                // Moves player and increase score
                playerSide = side::LEFT;
                playerScore++;

                timeRemaining += (2 / playerScore) + 0.15;
                
                axeSprite.setPosition(Vector2f(AXE_POSITION_LEFT, axeSprite.getPosition().y));
                playerSprite.setPosition(Vector2f(580.0f, 720.0f));

                updateBranches(playerScore);

                logSprite.setPosition(Vector2f(810.0f, 720.0f));
                
                logSpeedX = 5000;
                logActive = true;
                acceptInput = false;

                chop.play();
            }
        }

        window.clear();
        
        Time dt = clock.restart();

        if (!gamePaused) 
        {

            // Clock to determine speed for each iteration of loop
            // Time remaining subtracted by frame time
            timeRemaining -= dt.asSeconds();
            timeBar.setSize({ timeBarWidthPerSecond * timeRemaining, timeBarHeight });

            if (timeRemaining <= 0.0f) 
            {
                gamePaused = true;

                startingScreenText.setString("Game Over !!!");
                FloatRect textRect{ startingScreenText.getLocalBounds() };

                float width = textRect.size.x;
                float height = textRect.size.y;
                float left = textRect.position.x;
                float top = textRect.position.y;

                startingScreenText.setOrigin({ left + width / 2.0f, top + height / 2.0f });
                startingScreenText.setPosition({ 1920 / 2.0f, 1080 / 2.0f });

                
            }
           
            updateBee(beeSpeed, beeSprite, beeActive, dt);
            
            updateCloud(clouds, cloudsActive, cloudsSpeed, dt);

            // Updates player score text
            std::stringstream ss;
            ss << "Score = " << playerScore;
            scoreText.setString(ss.str());

            for (int i = 0; i < NUM_BRANCHES; i++) 
            {

                float height = static_cast<float>(i * 150);

                switch (branchPositions[i]) {
                case side::LEFT:
                    branches[i].setPosition(Vector2f(610.0f, height));
                    branches[i].setRotation(degrees(180.0f));
                    break;
                case side::RIGHT:
                    branches[i].setPosition(Vector2f(1330.0f, height));
                    branches[i].setRotation(degrees(0.0f));
                    break;
                default:
                    branches[i].setPosition(Vector2f(3000.0f, height));
                    break;
                }
            }
        }

        if (logActive)
        {

            logSprite.setPosition(Vector2f(logSprite.getPosition().x + (logSpeedX * dt.asSeconds())
                , logSprite.getPosition().y + (logSpeedY * dt.asSeconds())));

            if (logSprite.getPosition().x < -100 || logSprite.getPosition().y > 2000)
            {
                logActive = false;
                logSprite.setPosition(Vector2f(810.0f, 720.0f));
            }
        }
        
        if (branchPositions[5] == playerSide)
        {
            // Death
            gamePaused = true;
            acceptInput = false;

            ripSprite.setPosition(Vector2f(525.0f, 760.0f));
            playerSprite.setPosition(Vector2f(2000.0f, 660.0f));

            startingScreenText.setString("SQUISHED!!!");

            FloatRect textRect{ startingScreenText.getLocalBounds() };
            float width = textRect.size.x;
            float height = textRect.size.y;
            float left = textRect.position.x;
            float top = textRect.position.y;

            startingScreenText.setOrigin(Vector2f(left + width / 2.0f, top + height / 2.0f));
            startingScreenText.setPosition(Vector2f(1920.0f / 2.0f, 1080.0f / 2.0f));

           
        }

        window.draw(backgroundSprite);

        // Draw Clouds
        for (int i = 0; i < NUM_CLOUDS; i++)
        {
            window.draw(clouds[i]);
        }
        // Draw Branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }

        // Draw tree
        window.draw(treeSprite);

        window.draw(playerSprite);
        window.draw(axeSprite);
        window.draw(logSprite);
        window.draw(ripSprite);

        // Draw Bee
        window.draw(beeSprite);

        window.draw(timeBar);

        window.draw(scoreText);

        if (gamePaused) {
            window.draw(startingScreenText);
        }

        window.display();
    }

    return 0;
}

void updateBee(float& beeSpeed, Sprite& beeSprite, bool& beeActive, const Time& dt) {
    
    // Only runs once per bee cycle
    if (!beeActive)
    {
        // How fast is the bee
        srand((int)time(0));
        // Random number from 200-399 pixels
        beeSpeed = (rand() % 200) + 200;

        // Changes the height of the bee everytime it reaches the end
        srand((int)time(0) * 10);
        // Random number from 500-999 pixels
        float height = (rand() % 500) + 500;
        beeSprite.setPosition({ 2000, height });
        beeActive = true;
    }
    // Runs everytime and is what moves the sprite
    else
    {
        // Move bee   
        beeSprite.setPosition({ beeSprite.getPosition().x - (beeSpeed * dt.asSeconds()), beeSprite.getPosition().y });

        // Has the bee reached the left-hand edge of the screen
        if (beeSprite.getPosition().x < -100) {
            beeActive = false;
        }
    }
}
void updateCloud(std::vector<Sprite>& clouds, std::vector<bool>& cloudsActive, std::vector<float>& cloudsSpeed, const Time& dt) {

    for (int i = 0; i < NUM_CLOUDS; i++)
    {
        if (!cloudsActive[i])
        {
            // Speed
            cloudsSpeed[i] = ((rand() * i) % 200);

            // Height
            float height = (rand() % 150) + 10;
            clouds[i].setPosition(Vector2f(-200.0f, height));
            cloudsActive[i] = true;
        }
        else
        {
            // Move clouds position each frame
            clouds[i].setPosition(Vector2f(clouds[i].getPosition().x + (cloudsSpeed[i] * dt.asSeconds()), clouds[i].getPosition().y));
            // Disables cloud acitve when off screen
            if (clouds[i].getPosition().x > 1940)
            {
                cloudsActive[i] = false;
            }
        }
    }
}
void updateBranches(int seed)
{
    // Shifts old branches down
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }

    // Create random number from 0-4
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    // Generate new branches
    switch (r) {

    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    default:
        branchPositions[0] = side::NONE;
        break;
    }
}
 
