# ifndef _LOGICHANGMAN__H
# define _LOGICHANGMAN__H

# define START 0
# define GOOD_GUESS 1
# define BAD_GUESS 2
# define MAX_BAD_GUESSES 5

# include <iostream>

using namespace std;

struct Hangman {
    string secretWord;
    string guessedWord;
    int hiddenLetters;
    int badGuesses;
    int status = START;

    void init(const string& randomWord) {
        secretWord = randomWord;
        guessedWord = string(secretWord.length(), '_');
        badGuesses = 0;
        hiddenLetters = secretWord.length();
    }

    void update(char input) {
        status = BAD_GUESS;
        for (int i = 0; i < secretWord.length(); i++) {
            if (guessedWord[i] == '_' && secretWord[i] == input) {
                guessedWord[i] = input;
                status = GOOD_GUESS;
                hiddenLetters--;
            }
        }
        if (BAD_GUESS == status) {
            badGuesses++;
        }
    }

    bool lost() {
        return (badGuesses > MAX_BAD_GUESSES);
    }

    bool won() {
        return (hiddenLetters <= 0);
    }

    bool isOver() {
        return won() || lost();
    }
};

# endif // _LOGICHANGMAN__H
