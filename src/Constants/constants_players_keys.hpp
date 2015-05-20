#ifndef CONSTANTS_PLAYERS_KEYS
#define CONSTANTS_PLAYERS_KEYS

#include "constants.hpp"
#include <SDL/SDL.h>

int UP_KEYS[2] = {SDLK_i, SDLK_w};
int DOWN_KEYS[2] = {SDLK_k, SDLK_s};
int LEFT_KEYS[2] = {SDLK_j, SDLK_a};
int RIGHT_KEYS[2] = {SDLK_l, SDLK_d};
int BOMB_KEYS[2] = {SDLK_u, SDLK_r};

double PLAYERS_POSITIONS_X[2] = {30.0, FIELD_SIZE_X - 60.0};
double PLAYERS_POSITIONS_Y[2] = {30.0, FIELD_SIZE_Y - 60.0};

double DELTA = 0.25;

#endif