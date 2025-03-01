#include "game.h"
#include "renderer.h"
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include "include/SDL2/SDL_timer.h"

void drawHangman(SDL_Renderer *renderer, int attempts) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, 500, 500, 700, 500); // base
    SDL_RenderDrawLine(renderer, 600, 500, 600, 100); // pole
    SDL_RenderDrawLine(renderer, 600, 100, 750, 100); // top
    SDL_RenderDrawLine(renderer, 750, 100, 750, 150); // rope

    if (attempts <= 5) {
        SDL_Rect head = {725, 150, 50, 50};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &head);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, 735, 160, 745, 170); // left eye
        SDL_RenderDrawLine(renderer, 745, 160, 735, 170); // left eye
        SDL_RenderDrawLine(renderer, 755, 160, 765, 170); // right eye
        SDL_RenderDrawLine(renderer, 765, 160, 755, 170); // right eye
    }
    if (attempts <= 4) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, 750, 200, 750, 300); // body
    }
    if (attempts <= 3) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawLine(renderer, 750, 220, 725, 270); // left arm
    }
    if (attempts <= 2) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderDrawLine(renderer, 750, 220, 775, 270); // right arm
    }
    if (attempts <= 1) {
        SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
        SDL_RenderDrawLine(renderer, 750, 300, 725, 350); // left leg
    }
    if (attempts == 0) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        SDL_RenderDrawLine(renderer, 750, 300, 775, 350); // right leg
    }
}

void drawLargeHangman(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, 250, 500, 450, 500); // base
    SDL_RenderDrawLine(renderer, 350, 500, 350, 100); // pole
    SDL_RenderDrawLine(renderer, 350, 100, 500, 100); // top

    SDL_Rect head = {575, 350, 50, 50};
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &head);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, 585, 360, 595, 370); // left eye
    SDL_RenderDrawLine(renderer, 595, 360, 585, 370); // left eye
    SDL_RenderDrawLine(renderer, 605, 360, 615, 370); // right eye
    SDL_RenderDrawLine(renderer, 615, 360, 605, 370); // right eye
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderDrawLine(renderer, 600, 400, 600, 450); // body
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, 600, 420, 575, 470); // left arm
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawLine(renderer, 600, 420, 625, 470); // right arm
    SDL_SetRenderDrawColor(renderer, 255, 165, 0, 255);
    SDL_RenderDrawLine(renderer, 600, 450, 575, 500); // left leg
    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_RenderDrawLine(renderer, 600, 450, 625, 500); // right leg
}

void getInput(SDL_Renderer *renderer, TTF_Font *font, char *input, int maxLength) {
    SDL_StartTextInput();
    SDL_Event event;
    int running = 1;
    int length = 0;
    input[0] = '\0';
    char displayInput[maxLength];
    displayInput[0] = '\0';

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_TEXTINPUT) {
                if (length < maxLength - 1) {
                    strcat(input, event.text.text);
                    length += strlen(event.text.text);
                    strcat(displayInput, "*");
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    running = 0;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE && length > 0) {
                    input[--length] = '\0';
                    displayInput[length] = '\0';
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderText(renderer, font, "Enter the word to guess:", 50, 50);
        renderText(renderer, font, displayInput, 50, 100);
        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}

void runGame(SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont("ARIAL.TTF", 24);
    if (!font) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderText(renderer, font, "Welcome to The Hangman Game by Swopnil", 50, 50);
    renderText(renderer, font, "Press any key to start...", 50, 100);
    drawLargeHangman(renderer);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_KEYDOWN) {
            break;
        }
    }

    char word[100];
    getInput(renderer, font, word, 100);

    if (strlen(word) == 0) {
        printf("No word entered. Exiting...\n");
        TTF_CloseFont(font);
        return;
    }

    int wordLength = strlen(word);
    char guessedWord[wordLength + 1];
    memset(guessedWord, '_', wordLength);
    guessedWord[wordLength] = '\0';

    int attempts = 6;
    int running = 1;

    while (running && attempts > 0) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_KEYDOWN) {
                char guess = event.key.keysym.sym;
                guess = tolower(guess);
                if (guess >= 'a' && guess <= 'z') {
                    int correctGuess = 0;
                    for (int i = 0; i < wordLength; i++) {
                        if (tolower(word[i]) == guess) {
                            guessedWord[i] = word[i];
                            correctGuess = 1;
                        }
                    }
                    if (!correctGuess) {
                        attempts--;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        renderText(renderer, font, "Will you let an innocent soul perish?", 50, 20);
        renderText(renderer, font, guessedWord, 50, 300);
        renderText(renderer, font, "Attempts left: ", 50, 350);
        char attemptsStr[2];
        sprintf(attemptsStr, "%d", attempts);
        renderText(renderer, font, attemptsStr, 200, 350);

        renderText(renderer, font, "Can you guess it?", 50, 400);
        renderText(renderer, font, "Don't mess up!", 50, 450);

        drawHangman(renderer, attempts);

        SDL_RenderPresent(renderer);

        if (strcmp(word, guessedWord) == 0) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            renderText(renderer, font, "You won!", 50, 50);
            renderText(renderer, font, "Thanks for playing!", 50, 100);
            SDL_RenderPresent(renderer);
            SDL_Delay(3000);
            running = 0;
        }
    }

    if (attempts == 0) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderText(renderer, font, "You lost!", 50, 50);
        char lostMessage[128];
        sprintf(lostMessage, "The word was: %s", word);
        renderText(renderer, font, lostMessage, 50, 100);
        renderText(renderer, font, "Thanks for playing!", 50, 150);
        SDL_RenderPresent(renderer);
        SDL_Delay(3000);
    }

    TTF_CloseFont(font);
}