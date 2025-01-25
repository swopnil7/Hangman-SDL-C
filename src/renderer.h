#ifndef RENDERER_H
#define RENDERER_H

#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_ttf.h"

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);

#endif // RENDERER_H