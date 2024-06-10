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
struct EntityManager mgr_ = {0, {0}, {0}, {0}, {0}};
struct ComponentRegistry registry_ = {{{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0}}};


struct AppContext {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_bool app_quit;
};

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
    SDL_Window* window = SDL_CreateWindow("Window", 500, 500, SDL_WINDOW_RESIZABLE);
    if (!window){
        return SDL_Fail();
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
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

    // set up the application data
    // i have no idea if this works
    struct AppContext* appContext = malloc(sizeof(*appContext));
    appContext->window = window;
    appContext->renderer = renderer;
    appContext->app_quit = SDL_FALSE;

    appstate = (void**)&appContext;
    

    SDL_Log("Application started successfully!");


    // start program
    /*
    initEntityManager(&mgr_);

    pA = addEntity(&mgr_, 0);
    pB = addEntity(&mgr_, 0); 
    pC = addEntity(&mgr_, 0); 
    pD = addEntity(&mgr_, 0); 
    pE = addEntity(&mgr_, 0); 

    registry_.transforms_[pA] = (struct TransformComponent){50, 250, 0, 0};
    registry_.transforms_[pB] = (struct TransformComponent){75, 250, 0, 0};
    registry_.transforms_[pC] = (struct TransformComponent){100, 250, -10, -10};
    registry_.transforms_[pD] = (struct TransformComponent){200, 250, 10, 10};
    registry_.transforms_[pE] = (struct TransformComponent){300, 250, 0, -10};

    registry_.colors_[pA] = (struct ColorComponent){255, 255, 255, 255};
    registry_.colors_[pB] = (struct ColorComponent){50, 50, 50, 255};
    registry_.colors_[pC] = (struct ColorComponent){255, 0, 0, 255};
    registry_.colors_[pD] = (struct ColorComponent){0, 255, 0, 255};
    registry_.colors_[pE] = (struct ColorComponent){0, 0, 255, 255};
    */

    return 0;
}

int SDL_AppEvent(void *appstate, const SDL_Event* event) {
    struct AppContext* app = (struct AppContext*)appstate;
    
    if (event->type == SDL_EVENT_QUIT) {
        app->app_quit = SDL_TRUE;
    }

    return 0;
}

void SDL_AppQuit(void* appstate) {
    struct AppContext* app = (struct AppContext*)appstate;
    if (app) {
        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);
        free(app);
    }

    SDL_Quit();
    SDL_Log("Application quit successfully!");
}

int SDL_AppIterate(void *appstate) {
  struct AppContext* app = (struct AppContext*)appstate;
  /*
  SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 0);
  SDL_RenderClear(app->renderer);

  if (SDL_GetTicks() % 100 == 0) {
    updateTransforms(&mgr_, &registry_);
    renderColors(&mgr_, &registry_, app->renderer);
  }
  */

  return app->app_quit;
}
