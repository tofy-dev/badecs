#pragma once
#include <SDL3/SDL.h>
#include "entity_ecs.h"
#include "component_ecs.h"

#define bitat(N, I) 1==((N & (1l<<I)) >> I) // returns integer 0 or 1

void updateTransforms(struct EntityManager* mgr, struct ComponentRegistry* reg);

void renderColors(struct EntityManager* mgr, struct ComponentRegistry* reg, SDL_Renderer* renderer);

void updateAnimations(struct EntityManager* mgr, struct ComponentRegistry* reg);
