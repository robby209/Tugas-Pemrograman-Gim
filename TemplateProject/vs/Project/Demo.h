#ifndef DEMO_H
#define DEMO_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Music.h"
#include "Sound.h"
#include "Text.h"
#include <vector>

namespace Engine {
    enum class ScreenState {
        MAIN_MENU,
        IN_GAME
    };

    class Demo : public Engine::Game {
    public:
        Demo(Setting* setting);
        ~Demo();
        virtual void Init();
        virtual void Update();
        virtual void Render();

    private:
        void InitMainMenu();
        void InitInGame();
        void ResetGame();
        void UpdateMainMenu();
        void UpdateInGame();
        void RenderMainMenu();
        void RenderInGame();
        void GeneratePlatforms();

        Texture* dotTexture = nullptr;
        std::vector<Sprite*> platforms;
        Sprite* monsterSprite = nullptr;
        Sprite* backgroundSprite1 = nullptr;
        Sprite* backgroundSprite2 = nullptr;
        Sprite* dotSprite1 = nullptr;
        Sprite* dotSprite2 = nullptr;
        Sprite* dotSprite3 = nullptr;
        Sprite* dotSprite4 = nullptr;
        Music* music = nullptr;
        Sound* sound = nullptr;
        Text* text = nullptr;
        Text* titleText = nullptr;
        float yVelocity = 0, gravity = 0;
        bool jump = false, debug = false;

       
        float platformSpeed = 0.1f;
        float backgroundSpeed = 0.05f;

        
        int currentButtonIndex = 0;
        ScreenState screenState;
        std::vector<Button*> buttons;
    };
}

#endif
