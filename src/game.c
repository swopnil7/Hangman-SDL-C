#include "game.h"
#include "renderer.h"
#include <stdio.h>
#include <string.h>

void runGame(SDL_Renderer *renderer) {
    const char *word = "hangman";
    int wordLength = strlen(word);
    char guessedWord[wordLength + 1];
    memset(guessedWord, '_', wordLength);
    guessedWord[wordLength] = '\0';

    int attempts = 6;
    int running = 1;
    SDL_Event event;

    TTF_Font *font = TTF_OpenFont("path/to/font.ttf", 24);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return;
    }

    while (running && attempts > 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                char guess = event.key.keysym.sym;
                int correctGuess = 0;
                for (int i = 0; i < wordLength; i++) {
                    if (word[i] == guess) {
                        guessedWord[i] = guess;
                        correctGuess = 1;
                    }
                }
                if (!correctGuess) {
                    attempts--;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderText(renderer, font, guessedWord, 50, 300);
        renderText(renderer, font, "Attempts left: ", 50, 350);
        char attemptsStr[2];
        sprintf(attemptsStr, "%d", attempts);
        renderText(renderer, font, attemptsStr, 200, 350);

        SDL_RenderPresent(renderer);

        if (strcmp(word, guessedWord) == 0) {
            printf("You won!\n");
            running = 0;
        }
    }

    if (attempts == 0) {
        printf("You lost! The word was: %s\n", word);
    }

    TTF_CloseFont(font);
}