#include "src/include/SDL2/SDL.h"
#include "src/include/SDL2/SDL_ttf.h"
#include <stdio.h>
#include <string.h>

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    SDL_Color color = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Text rendering failed! TTF_Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Texture creation failed! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);
    SDL_DestroyTexture(texture);
}

void drawHangman(SDL_Renderer *renderer, int attempts) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Draw the gallows
    SDL_RenderDrawLine(renderer, 150, 500, 450, 500); // Base
    SDL_RenderDrawLine(renderer, 300, 500, 300, 100); // Pole
    SDL_RenderDrawLine(renderer, 300, 100, 400, 100); // Top
    SDL_RenderDrawLine(renderer, 400, 100, 400, 150); // Rope

    // Draw the hangman based on the number of attempts left
    if (attempts <= 5) {
        SDL_RenderDrawLine(renderer, 400, 150, 400, 250); // Body
    }
    if (attempts <= 4) {
        SDL_RenderDrawLine(renderer, 400, 200, 350, 150); // Left arm
    }
    if (attempts <= 3) {
        SDL_RenderDrawLine(renderer, 400, 200, 450, 150); // Right arm
    }
    if (attempts <= 2) {
        SDL_RenderDrawLine(renderer, 400, 250, 350, 300); // Left leg
    }
    if (attempts <= 1) {
        SDL_RenderDrawLine(renderer, 400, 250, 450, 300); // Right leg
    }
    if (attempts == 0) {
        SDL_Rect head = {375, 150, 50, 50};
        SDL_RenderDrawRect(renderer, &head); // Head
    }
}

void runGame(SDL_Renderer *renderer) {
    const char *word = "hangman";
    int wordLength = strlen(word);
    char guessedWord[wordLength + 1];
    memset(guessedWord, '_', wordLength);
    guessedWord[wordLength] = '\0';

    int attempts = 6;
    int running = 1;
    SDL_Event event;

    TTF_Font *font = TTF_OpenFont("ARIAL.TTF", 24);
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

        drawHangman(renderer, attempts);

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

    // Wait for the user to close the window
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! TTF_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Hangman Game", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        800, 600, 
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    runGame(renderer);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}