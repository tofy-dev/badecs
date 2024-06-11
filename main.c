#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

#include "component_ecs.h"
#include "system_ecs.h"
#include "entity_ecs.h"

// copy boilerplate
// dont forget to initalize registry
Entity pA, pB, pC, pD, pE;
struct EntityManager manager = {0, {0}, {0}, {0}, {0}};
struct ComponentRegistry registry = {{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0}}};

SDL_Window* window;
SDL_Renderer* renderer;
SDL_bool app_quit;


int SDL_Fail(){
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error %s", SDL_GetError());
    return -1;
}

int SDL_AppInit(void** appstate, int argc, char* argv[]) {
    // init the library, here we make a window so we only need the Video capabilities.
    if (SDL_Init(SDL_INIT_VIDEO)){
        return SDL_Fail();
    }
    
    // create a window
    window = SDL_CreateWindow("Window", 500, 500, SDL_WINDOW_RESIZABLE);
    if (!window){
        return SDL_Fail();
    }
    
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer){
        return SDL_Fail();
    }
    
    // print some information about the window
    SDL_ShowWindow(window);
    {
        int width, height, bbwidth, bbheight;
        SDL_GetWindowSize(window, &width, &height);
        SDL_GetWindowSizeInPixels(window, &bbwidth, &bbheight);
        SDL_Log("Window size: %ix%i", width, height);
        SDL_Log("Backbuffer size: %ix%i", bbwidth, bbheight);
        if (width != bbwidth){
            SDL_Log("This is a highdpi environment.");
        }
    }

    SDL_Log("Application started successfully!");


    // start program
    initEntityManager(&manager);

    pA = addEntity(&manager, TRANS|COLOR);
    pB = addEntity(&manager, TRANS|COLOR); 
    pC = addEntity(&manager, TRANS|COLOR); 
    pD = addEntity(&manager, TRANS|COLOR); 
    pE = addEntity(&manager, TRANS|COLOR); 

    // should accept entity_to_index_ value
    registry.transforms_[manager.entity_to_idx_[pA]] = (struct TransformComponent){50, 250, 0, 0};
    registry.transforms_[manager.entity_to_idx_[pB]] = (struct TransformComponent){75, 250, 0, 0};
    registry.transforms_[manager.entity_to_idx_[pC]] = (struct TransformComponent){100, 250, -10, -10};
    registry.transforms_[manager.entity_to_idx_[pD]] = (struct TransformComponent){200, 250, 10, 10};
    registry.transforms_[manager.entity_to_idx_[pE]] = (struct TransformComponent){300, 250, 0, -10};

    registry.colors_[manager.entity_to_idx_[pA]] = (struct ColorComponent){255, 255, 255, 255};
    registry.colors_[manager.entity_to_idx_[pB]] = (struct ColorComponent){50, 50, 50, 255};
    registry.colors_[manager.entity_to_idx_[pC]] = (struct ColorComponent){255, 0, 0, 255};
    registry.colors_[manager.entity_to_idx_[pD]] = (struct ColorComponent){0, 255, 0, 255};
    registry.colors_[manager.entity_to_idx_[pE]] = (struct ColorComponent){0, 0, 255, 255};

    return 0;
}

int SDL_AppEvent(void *appstate, const SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) {
        app_quit = SDL_TRUE;
    }

    return 0;
}

void SDL_AppQuit(void* appstate) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    SDL_Log("Application quit successfully!");
}

int SDL_AppIterate(void *appstate) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);

  if (SDL_GetTicks() % 10 == 0) {
    updateTransforms(&manager, &registry);
    renderColors(&manager, &registry, renderer);
    updateCollisions(&manager, &registry);
  }

  return app_quit;
}
