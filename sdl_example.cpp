/*
https://wiki.libsdl.org/SDL2/APIByCategory
https://wiki.libsdl.org/SDL2/CategoryAPI
*/
#include <SDL2/SDL.h>
#include <emscripten.h>

const int WINDOW_WIDTH = 300, WINDOW_HEIGHT = 300;

SDL_Window *window;
SDL_Renderer *renderer;

/*
bool handle_events() {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return false;
  }
  return true;
}
*/

void setup() {
  SDL_Init(SDL_INIT_VIDEO);
  // uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
  int flags = 0;
  SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, flags, &window,
                              &renderer);
}

void draw() {
  // set black background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  // draw green rect
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_Rect rect = {.x = 10, .y = 10, .w = 150, .h = 100};
  SDL_RenderFillRect(renderer, &rect);
  // Update the screen with any rendering performed since the previous call.
  SDL_RenderPresent(renderer);
}

void quit() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

// void main_loop() { handle_events(); }

int main() {
  setup();

  const int fps = 0;
  const int simulate_infinite_loop = 1;
  emscripten_set_main_loop(draw, fps, simulate_infinite_loop);

  quit();

  return 0;
}
