#include "renderer.h"
#include <stdio.h>

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
    if (text == NULL || text[0] == '\0') {
        return;
    }

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Text rendering failed! TTF_Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Texture creation failed! SDL_Error: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}