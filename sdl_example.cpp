/*
https://wiki.libsdl.org/SDL2/APIByCategory
https://wiki.libsdl.org/SDL2/CategoryAPI
*/
#include <SDL2/SDL.h>
#include <emscripten.h>

const int WINDOW_WIDTH = 300, WINDOW_HEIGHT = 300;

SDL_Window *window;
SDL_Renderer *renderer;

bool rect_color_flag = true;

void toggle_rect_color() { rect_color_flag = !rect_color_flag; }

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
  if (rect_color_flag) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  }
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

bool handle_events() {
  SDL_Event event;
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    return false;
  }
  if (event.type == SDL_MOUSEBUTTONDOWN) {
    if (event.button.button == SDL_BUTTON_LEFT) {
      toggle_rect_color();
    }
    draw();
  }
  return true;
}

void main_loop() { handle_events(); }

int main() {
  setup();

  draw();

  const int fps = 0;
  const int simulate_infinite_loop = 1;
  emscripten_set_main_loop(main_loop, fps, simulate_infinite_loop);

  quit();

  return 0;
}
