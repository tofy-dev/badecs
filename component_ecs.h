#pragma once
#include <SDL3/SDL.h>
#include "entity_ecs.h"

// COMPONENT DEFINITIONS
struct TransformComponent {
  float pos_x;
  float pos_y;
  float vel_x;
  float vel_y;
};

struct ColorComponent {
  Uint8 r;
  Uint8 g;
  Uint8 b;
  Uint8 a;
};

struct AnimationComponent {
  float delay;
};


// REGISTRY (stores all component data)
struct ComponentRegistry {
  struct TransformComponent transforms_[MAX_ENTITIES];
  struct ColorComponent colors_[MAX_ENTITIES];
  struct AnimationComponent animations_[MAX_ENTITIES];
};


// COMPONENT MACROS
#define TRANS (1l << 0)
#define COLOR (1l << 1)
#define ANIMA (1l << 2)
