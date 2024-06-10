#include <SDL3/SDL.h>
#include "system_ecs.h"
#include "entity_ecs.h"
#include "component_ecs.h"

void updateTransforms(struct EntityManager* mgr, struct ComponentRegistry* reg) {
  for (int i = 0; i < mgr->entity_count_; i++) {
    Entity entity = mgr->idx_to_entity_[i];
    size_t idx = mgr->entity_to_idx_[entity];

    if (!(bitat(mgr->component_tracker_[idx], TRANS))) continue;

    reg->transforms_[i].pos_y += reg->transforms_[i].vel_y*0.1;
    reg->transforms_[i].pos_x += reg->transforms_[i].vel_x*0.1;
  }
}

void renderColors(struct EntityManager* mgr, struct ComponentRegistry* reg, SDL_Renderer* renderer) {
  for (int i = 0; i < mgr->entity_count_; i++) {
    Entity entity = mgr->idx_to_entity_[i];
    size_t idx = mgr->entity_to_idx_[entity];

    if (!(bitat(mgr->component_tracker_[idx], TRANS))) continue;
    if (!(bitat(mgr->component_tracker_[idx], COLOR))) continue;

    SDL_SetRenderDrawColor(renderer, reg->colors_[i].r, reg->colors_[i].g, reg->colors_[i].b, reg->colors_[i].a);
    SDL_FRect rect = {reg->transforms_[i].pos_x, reg->transforms_[i].pos_y, 10, 10};
    SDL_RenderRect(renderer, &rect);
  }
  SDL_RenderPresent(renderer);
}

void updateAnimations(struct EntityManager* mgr, struct ComponentRegistry* reg) {
  // do nothing
}
