# include <iostream>
# include <ctime>
# include "graphicsHangman.h"

using namespace std;

const string WORD_LIST[] = {"abaa", "aaba"};
const int WORD_COUNT = sizeof(WORD_LIST) / sizeof(string);

const string& chooseWord()
{
    int randomIndex = rand() % WORD_COUNT;
    return WORD_LIST[randomIndex];
}

int main(int argc, char ** argv) {
    srand(time(0));
    Graphics graphics;
    graphics.init();

    Hangman game;
    game.init(chooseWord());
    graphics.start(game);
    graphics.render(game);

    SDL_Event button;
    bool quit = false;
    char input;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    while(!quit){
        SDL_PollEvent( &button);
        switch( button.type){
            case SDL_QUIT:
                quit = true;
                break;
        }
        if (currentKeyStates[SDL_SCANCODE_A] ){
            input = 'a';
            game.update(input);
            graphics.render(game);
            quit = game.isOver();
        }
        if (currentKeyStates[SDL_SCANCODE_B] ){
            input = 'b';
            game.update(input);
            graphics.render(game);
            quit = game.isOver();
        }
    }

    graphics.endgame(game);

    graphics.quit();
    return 0;
}
