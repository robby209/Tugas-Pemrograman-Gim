#include "Demo.h"
#include <cstdlib>  
#include <ctime>   
#include <random>

using namespace Engine;

Demo::Demo(Setting* setting) : Game(setting) {
    screenState = ScreenState::MAIN_MENU;
    text = nullptr;
    srand(static_cast<unsigned int>(time(0)));  
}

Demo::~Demo() {
    delete dotTexture;
    delete monsterSprite;
    delete dotSprite1;
    delete dotSprite2;
    delete dotSprite3;
    delete dotSprite4;
    delete music;
    delete text;
    delete titleText; 
    delete backgroundSprite1;
    delete backgroundSprite2;

    for (Sprite* platform : platforms) {
        delete platform;
    }
    platforms.clear();
}

void Demo::Init() {
    InitMainMenu();
    InitInGame();
}

void Demo::Update() {
    if (screenState == ScreenState::MAIN_MENU) {
        UpdateMainMenu();
    }
    else if (screenState == ScreenState::IN_GAME) {
        UpdateInGame();
    }
}

void Demo::Render() {
    if (screenState == ScreenState::MAIN_MENU) {
        RenderMainMenu();
    }
    else if (screenState == ScreenState::IN_GAME) {
        RenderInGame();
    }
}

void Demo::InitMainMenu() {
    // Create background
    Texture* bgTexture = new Texture("Tempat.png");
    backgroundSprite1 = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
    backgroundSprite1->SetSize(setting->screenWidth, setting->screenHeight);
    backgroundSprite1->SetPosition(0, 0);

    backgroundSprite2 = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
    backgroundSprite2->SetSize(setting->screenWidth, setting->screenHeight);
    backgroundSprite2->SetPosition(setting->screenWidth, 0);

    // Create a Texture for buttons
    Texture* texture = new Texture("buttons.png");

    // Create Sprites
    Sprite* playSprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
    playSprite->SetNumXFrames(6);
    playSprite->SetNumYFrames(1);
    playSprite->AddAnimation("normal", 5, 5);
    playSprite->AddAnimation("hover", 3, 4);
    playSprite->AddAnimation("press", 3, 4);
    playSprite->SetAnimationDuration(400);

    Sprite* exitSprite = new Sprite(texture, defaultSpriteShader, defaultQuad);
    exitSprite->SetNumXFrames(6);
    exitSprite->SetNumYFrames(1);
    exitSprite->AddAnimation("normal", 2, 2);
    exitSprite->AddAnimation("hover", 0, 1);
    exitSprite->AddAnimation("press", 0, 1);
    exitSprite->SetAnimationDuration(400);

    // Create Buttons
    Button* playButton = new Button(playSprite, "play");
    playButton->SetPosition((setting->screenWidth / 2) - (playSprite->GetScaleWidth() / 2), 400);
    buttons.push_back(playButton);

    Button* exitButton = new Button(exitSprite, "exit");
    exitButton->SetPosition((setting->screenWidth / 2) - (exitSprite->GetScaleWidth() / 2), 250);
    buttons.push_back(exitButton);

    // Set play button into active button
    currentButtonIndex = 0;
    buttons[currentButtonIndex]->SetButtonState(ButtonState::HOVER);

    // Create Text for title
    titleText = new Text("8-bit Arcade In.ttf", 100, defaultTextShader);
    titleText->SetText("Turtle Flight");
    titleText->SetPosition((setting->screenWidth / 2) - 250, setting->screenHeight - 200);
    titleText->SetColor(235, 229, 52);

    // Add input mappings
    inputManager->AddInputMapping("next", SDLK_DOWN);
    inputManager->AddInputMapping("prev", SDLK_UP);
    inputManager->AddInputMapping("press", SDLK_RETURN);
}

void Demo::InitInGame() {
    // Generate Platforms
    GeneratePlatforms();

    // Create background
    Texture* bgTexture = new Texture("Tempat.png");
    backgroundSprite1 = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
    backgroundSprite1->SetSize(setting->screenWidth, setting->screenHeight);
    backgroundSprite1->SetPosition(0, 0);

    backgroundSprite2 = new Sprite(bgTexture, defaultSpriteShader, defaultQuad);
    backgroundSprite2->SetSize(setting->screenWidth, setting->screenHeight);
    backgroundSprite2->SetPosition(setting->screenWidth, 0);

    // Create monster sprite
    Texture* monsterTexture = new Texture("Attack.png");
    monsterSprite = new Sprite(monsterTexture, defaultSpriteShader, defaultQuad);
    monsterSprite->SetPosition(setting->screenWidth / 2 - monsterSprite->GetScaleWidth() / 2, setting->screenHeight / 2 - monsterSprite->GetScaleHeight() / 2); // Set initial position
    monsterSprite->SetNumXFrames(6);
    monsterSprite->SetNumYFrames(1);
    monsterSprite->SetAnimationDuration(130);
    monsterSprite->SetScale(3.0f);
    monsterSprite->AddAnimation("idle", 0, 5);
    monsterSprite->SetBoundingBoxSize(monsterSprite->GetScaleWidth() - (16 * monsterSprite->GetScale()),
        monsterSprite->GetScaleHeight() - (4 * monsterSprite->GetScale()));

    // Create debug dots
    dotTexture = new Texture("");
    dotSprite1 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);
    dotSprite2 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);
    dotSprite3 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);
    dotSprite4 = new Sprite(dotTexture, defaultSpriteShader, defaultQuad);

    // Add input mapping
    inputManager->AddInputMapping("Flap", SDLK_SPACE);
    inputManager->AddInputMapping("Run Right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    inputManager->AddInputMapping("Run Left", SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    inputManager->AddInputMapping("Quit", SDLK_ESCAPE);

    // Playing music
    music = new Music("2021-08-16_-_8_Bit_Adventure_-_www.FesliyanStudios.com.ogg");
    music->SetVolume(40);
    music->Play(true);

    sound = new Sound("jump.wav");
    sound->SetVolume(100);

    // Create Text for in-game instructions
    text = new Text("lucon.ttf", 24, defaultTextShader);
    text->SetScale(1.0f);
    text->SetColor(255, 255, 255);
    text->SetPosition(10, setting->screenHeight - 30);  // Adjusted position

    // Set debug
    debug = true;
}

void Demo::ResetGame() {
    monsterSprite->SetPosition(setting->screenWidth / 2 - monsterSprite->GetScaleWidth() / 2,
        setting->screenHeight / 2 - monsterSprite->GetScaleHeight() / 2);
    yVelocity = 0;

    
    jump = false;

    
    GeneratePlatforms();

    
    backgroundSprite1->SetPosition(0, 0);
    backgroundSprite2->SetPosition(setting->screenWidth, 0);
}

void Demo::GeneratePlatforms() {
    platforms.clear();  

    Texture* platformTexture = new Texture("objek2.png");  
    int platformWidth = 200;  
    int verticalGap = 300;    
    int numberOfPairs = 40;   

   
    int minHeight = 100;
    int maxHeight = 500;
    std::random_device rd;  
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uniHeight(minHeight, maxHeight);

    float initialX = setting->screenWidth;  

    for (int i = 0; i < numberOfPairs; i++) {
        int topHeight = uniHeight(rng);
        float posX = initialX + i * (platformWidth + 300);  

        
        Sprite* topPlatform = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
        topPlatform->SetPosition(posX, 0);
        topPlatform->SetSize(platformWidth, topHeight);
        topPlatform->SetBoundingBoxSize(platformWidth, topHeight);
        platforms.push_back(topPlatform);

        
        int bottomHeight = setting->screenHeight - (topHeight + verticalGap);
        Sprite* bottomPlatform = new Sprite(platformTexture, defaultSpriteShader, defaultQuad);
        bottomPlatform->SetPosition(posX, setting->screenHeight - bottomHeight);
        bottomPlatform->SetSize(platformWidth, bottomHeight);
        bottomPlatform->SetBoundingBoxSize(platformWidth, bottomHeight);
        platforms.push_back(bottomPlatform);
    }
}

void Demo::UpdateMainMenu() {
    
    vec2 bgPos1 = backgroundSprite1->GetPosition();
    vec2 bgPos2 = backgroundSprite2->GetPosition();
    bgPos1.x -= backgroundSpeed * GetGameTime();
    bgPos2.x -= backgroundSpeed * GetGameTime();

    
    if (bgPos1.x + backgroundSprite1->GetScaleWidth() <= 0) {
        bgPos1.x = bgPos2.x + backgroundSprite2->GetScaleWidth();
    }
    
    if (bgPos2.x + backgroundSprite2->GetScaleWidth() <= 0) {
        bgPos2.x = bgPos1.x + backgroundSprite1->GetScaleWidth();
    }

    backgroundSprite1->SetPosition(bgPos1.x, bgPos1.y);
    backgroundSprite2->SetPosition(bgPos2.x, bgPos2.y);

    if (inputManager->IsKeyReleased("next")) {
        buttons[currentButtonIndex]->SetButtonState(ButtonState::NORMAL);
        currentButtonIndex = currentButtonIndex < buttons.size() - 1 ? currentButtonIndex + 1 : currentButtonIndex;
        buttons[currentButtonIndex]->SetButtonState(ButtonState::HOVER);
    }

    if (inputManager->IsKeyReleased("prev")) {
        buttons[currentButtonIndex]->SetButtonState(ButtonState::NORMAL);
        currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
        buttons[currentButtonIndex]->SetButtonState(ButtonState::HOVER);
    }

    if (inputManager->IsKeyReleased("press")) {
        Button* b = buttons[currentButtonIndex];
        b->SetButtonState(ButtonState::PRESS);
        if ("play" == b->GetButtonName()) {
            ResetGame(); 
            screenState = ScreenState::IN_GAME;
        }
        else if ("exit" == b->GetButtonName()) {
            state = State::EXIT;
        }
    }

    // Update All buttons
    for (Button* b : buttons) {
        b->Update(GetGameTime());
    }
}

void Demo::UpdateInGame() {
    if (inputManager->IsKeyReleased("Quit")) {
        screenState = ScreenState::MAIN_MENU;
    }

    text->SetText("[FPS = " + to_string(currentFrameRate) + "] Space to Jump");

    monsterSprite->PlayAnim("idle");

    vec2 oldMonsterPos = monsterSprite->GetPosition();
    float x = oldMonsterPos.x, y = oldMonsterPos.y;

    monsterSprite->SetPosition(x, y);

    for (Sprite* s : platforms) {
        if (s->GetBoundingBox()->CollideWith(monsterSprite->GetBoundingBox())) {
            screenState = ScreenState::MAIN_MENU; 
            return; 
        }
    }

   
    if (inputManager->IsKeyPressed("Flap")) {
        yVelocity = 0.5f; 
        sound->Play(false); 
    }

    yVelocity -= 0.02f; 
    y += yVelocity * GetGameTime();
    monsterSprite->SetPosition(x, y);

    if (y < 0) {
        y = 0;
        yVelocity = 0;
        screenState = ScreenState::MAIN_MENU; 
    }

   
    if (y > setting->screenHeight) {
        y = setting->screenHeight;
        yVelocity = 0;
        screenState = ScreenState::MAIN_MENU;
    }

    if (inputManager->IsKeyPressed("Attack")) {
        monsterSprite->PlayAnim("attack");
    }

   
    std::vector<Sprite*> remainingPlatforms;
    for (Sprite* platform : platforms) {
        vec2 platformPos = platform->GetPosition();
        platformPos.x -= platformSpeed * GetGameTime();

        if (platformPos.x + platform->GetScaleWidth() > 0) {
            remainingPlatforms.push_back(platform);
            platform->SetPosition(platformPos.x, platformPos.y);
        }
        else {
            delete platform; 
        }
    }
    platforms = remainingPlatforms; 
    
    vec2 bgPos1 = backgroundSprite1->GetPosition();
    vec2 bgPos2 = backgroundSprite2->GetPosition();
    bgPos1.x -= backgroundSpeed * GetGameTime();
    bgPos2.x -= backgroundSpeed * GetGameTime();

    
    if (bgPos1.x + backgroundSprite1->GetScaleWidth() <= 0) {
        bgPos1.x = bgPos2.x + backgroundSprite2->GetScaleWidth();
    }
  
    if (bgPos2.x + backgroundSprite2->GetScaleWidth() <= 0) {
        bgPos2.x = bgPos1.x + backgroundSprite1->GetScaleWidth();
    }

    backgroundSprite1->SetPosition(bgPos1.x, bgPos1.y);
    backgroundSprite2->SetPosition(bgPos2.x, bgPos2.y);

    monsterSprite->Update(GetGameTime());

    if (debug) {
        BoundingBox* bb = monsterSprite->GetBoundingBox();
        dotSprite1->SetPosition(bb->GetVertices()[0].x - (0.5f * dotSprite1->GetScaleWidth()),
            bb->GetVertices()[0].y - (0.5f * dotSprite1->GetScaleHeight()));
        dotSprite2->SetPosition(bb->GetVertices()[1].x - (0.5f * dotSprite2->GetScaleWidth()),
            bb->GetVertices()[1].y - (0.5f * dotSprite2->GetScaleHeight()));
        dotSprite3->SetPosition(bb->GetVertices()[2].x - (0.5f * dotSprite3->GetScaleWidth()),
            bb->GetVertices()[2].y - (0.5f * dotSprite3->GetScaleHeight()));
        dotSprite4->SetPosition(bb->GetVertices()[3].x - (0.5f * dotSprite4->GetScaleWidth()),
            bb->GetVertices()[3].y - (0.5f * dotSprite4->GetScaleHeight()));
    }
}

void Demo::RenderMainMenu() {
   
    backgroundSprite1->Draw();
    backgroundSprite2->Draw();

    titleText->Draw();
    text->Draw();

    for (Button* b : buttons) {
        b->Draw();
    }

    text->Draw();
}

void Demo::RenderInGame() {
 
    backgroundSprite1->Draw();
    backgroundSprite2->Draw();

    for (Sprite* s : platforms) {
        s->Draw();
    }
    monsterSprite->Draw();
    text->Draw();

    if (debug) {
        dotSprite1->Draw();
        dotSprite2->Draw();
        dotSprite3->Draw();
        dotSprite4->Draw();
    }
}

