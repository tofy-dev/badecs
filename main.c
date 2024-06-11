#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>
#include <time.h>

#include "component_ecs.h"
#include "system_ecs.h"
#include "entity_ecs.h"

// copy boilerplate
// dont forget to initalize registry
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
    srand(time(NULL));

    for (int i = 0; i < 1000; i++) {
      Entity ent = addEntity(&manager, TRANS|COLOR);
      registry.transforms_[manager.entity_to_idx_[ent]] = (struct TransformComponent){rand()%501, rand()%501, rand()%10, rand()%10};
      registry.colors_[manager.entity_to_idx_[ent]] = (struct ColorComponent){rand()%256, rand()%256, rand()%256, 255};
    }

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
