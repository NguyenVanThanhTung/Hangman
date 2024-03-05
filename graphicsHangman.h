# ifndef _GRAPHICSHANGMAN__H
# define _GRAPHICSHANGMAN__H

# include <iostream>
# include <ctime>
# include <SDL.h>
# include <SDL_image.h>
# include "defsHangman.h"
# include "logicHangman.h"

void waitUntillPressed(){
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type ==  SDL_QUIT || e.type == SDL_MOUSEBUTTONDOWN) )
            return;
    }
}

struct Graphics{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *background, *A, *B, *defeat, *victory, *badguesses, *goodguesses, *wrong, *_;

    void logErrorAndExit(const char* msg, const char* error){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
        SDL_Quit();
    }

    void initSDL(){
        if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
            logErrorAndExit("SDL_Init", SDL_GetError());
        }
        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr){
            logErrorAndExit("CreateWindow", SDL_GetError());
        }

        if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG)){
            logErrorAndExit( "SDL_image error:", IMG_GetError());
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (renderer == nullptr){
             logErrorAndExit("CreateRenderer", SDL_GetError());
        }
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    void init(){
         initSDL();
         background = loadTexture("background.png");
         A = loadTexture ("A.png");
         B = loadTexture ("B.png");
         defeat = loadTexture ("defeat.png");
         victory = loadTexture ("victory.png");
         badguesses = loadTexture ("badguesses.png");
         goodguesses = loadTexture ("goodguesses.png");
         wrong = loadTexture ("wrong.png");
         _ = loadTexture("_.png");
    }

    void prepareScene(SDL_Texture * background){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
    }

    void presentScene(){
        SDL_RenderPresent(renderer);
    }

    SDL_Texture *loadTexture(const char *filename){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO,
                       "Loading %s", filename);
        SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
        if (texture == NULL)
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                  SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());

        return texture;
    }

    void renderTexture(SDL_Texture *texture, int x, int y){
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, NULL, &dest);
    }

    void start(const Hangman& game){
        renderTexture(background, 0, 0);
        presentScene();
    }

    void render(Hangman& game) {
        for(int i=0; i< game.secretWord.length(); i++){
            switch(game.guessedWord[i]){
                case '_':
                    renderTexture(_, 150 + CHAR_LENGTH*i, 225);
                    break;
                case 'a':
                    renderTexture(A, 150 + CHAR_LENGTH*i, 225);
                    break;
                case 'b':
                    renderTexture(B, 150 + CHAR_LENGTH*i, 225);
                    break;
            }
        }

        switch (game.status){
            case GOOD_GUESS:
                renderTexture(goodguesses, 150, 425);
                break;
            case BAD_GUESS:
                renderTexture(badguesses, 150, 425);
        }
        renderTexture(wrong, 150, 325);
        presentScene();
        waitUntillPressed();
    }

    void endgame(Hangman game){
        if (game.won()){
            renderTexture(victory, 0, 0);
        } else {
            if (game.lost()){
            renderTexture(defeat, 0, 0);
            }
        }
        presentScene();
        waitUntillPressed();
    }

    void quit()
    {
        SDL_DestroyTexture(background);
        background = nullptr;
        SDL_DestroyTexture(A);
        A = nullptr;
        SDL_DestroyTexture(B);
        B = nullptr;
        SDL_DestroyTexture(defeat);
        defeat = nullptr;
        SDL_DestroyTexture(victory);
        victory = nullptr;
        SDL_DestroyTexture(badguesses);
        badguesses = nullptr;
        SDL_DestroyTexture(goodguesses);
        goodguesses = nullptr;
        SDL_DestroyTexture(wrong);
        wrong = nullptr;
        SDL_DestroyTexture(_);
        _ = nullptr;

        IMG_Quit();

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

# endif // _GRAPHICSHANGMAN__H
